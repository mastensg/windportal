# coding=utf-8

import gevent.monkey
gevent.monkey.patch_all() # make sure all syncronous calls in stdlib yields to gevent
import paho.mqtt.client as mqtt
import lxml.html

import urllib.request
import urllib.parse
import os
import json
import datetime
import sys
import time
import xml.etree.ElementTree
import math
import numbers
import pprint
from urllib.parse import urlparse

import logging
logging.basicConfig()
log_level = os.environ.get('LOGLEVEL', 'info')
log = logging.getLogger('server')
log.setLevel(getattr(logging, log_level.upper()))


def assert_valid_windspeed(w):
    assert isinstance(w, numbers.Number), "Windspeed not a number: {}".format(type(w))
    assert 0.0 <= w < 40.0, "Invalid {}".format(w)
    return

def windspeed_ukenergy():
    url = 'https://ukenergy.statoil.com/wind'
    d = urllib.request.urlopen(url).read()
    root = lxml.html.fromstring(d)

    # data-windspeed is in meters/second
    # speed-label is shown by JS as miles-per-hours (at least with en_US locale)
    data_windspeed = root.xpath("//div[@id='wind-widget']/@data-windspeed")
    assert len(data_windspeed) == 1, 'Could not find element with data-windspeed'
    windspeed = float(data_windspeed[0])

    assert_valid_windspeed(windspeed)
    return windspeed

def create_mqtt_client(broker_url):
    broker_info = urlparse(broker_url)

    client = mqtt.Client()
    if broker_info.username:
        client.username_pw_set(broker_info.username, broker_info.password)

    client.reconnect_delay_set(min_delay=0.1, max_delay=2*60)

    host = broker_info.hostname
    default_port = 1883
    if broker_info.scheme == 'mqtts':
        default_port = 8883
        client.tls_set()
    port = broker_info.port or default_port

    # XXX loop() does not handle reconnects, have to use loop_start() or loop_forever() 
    client.loop_start()

    return client, host, port



def seen_since(messages, time : float):
    devices = {}
    for m in messages:
        d = m['payload']['role']
        t = m['time_received']
        if t > time:
            devices[d] = t
    return devices

def setup_app(broker_url, check_interval=30*60, status_interval=10, done_cb=None):
    wind_speed = 0.1111
    wind_speed_updated_at_fmt = datetime.datetime.now().replace(microsecond=0).isoformat()
    wind_speed_updated_at_time = time.time()
    heartbeat_messages = []
    running = True

    def write_status_page():
        nonlocal wind_speed
        nonlocal wind_speed_updated_at_fmt
        nonlocal wind_speed_updated_at_time
        nonlocal heartbeat_messages

        log.info('Writing status page')

        now = time.time()

        def format_heartbeat_message(m):
            t = m["time_received"]
            lt = time.localtime(t)
            tf = time.strftime("%Y-%m-%dT%H:%M:%SZ", lt)

            dt = now - t
            dtf = str(datetime.timedelta(seconds=dt))

            mf = pprint.pformat(m)

            return """
{} ({} ago)

{}
""".format(
        tf,
        dtf,
        mf
        )

        with open("/opt/windportal/html/index.html", "w") as f:
            nowf = datetime.datetime.now().replace(microsecond=0).isoformat()

            status = """<h1 style="color: red">Offline</h1>"""

            dt = now - wind_speed_updated_at_time
            wind_speed_updated_at_ago = str(datetime.timedelta(seconds=dt))

            if 0 < len(heartbeat_messages):
                m = heartbeat_messages[-1]
                t = m["time_received"]
                dt = now - t

                if dt < 60.0:
                    status = """<h1 style="color: green">Online</h1>"""

            doc = """
<title>windportal</title>
{}
<pre>
last updated: {} UTC

wind_speed:   {} m/s at {} ({} ago)

<hr />
last {} heartbeat_messages:
{}
""".format(
        status,
        nowf,
        wind_speed,
        wind_speed_updated_at_fmt,
        wind_speed_updated_at_ago,
        len(heartbeat_messages),
        "\n\n".join(map(format_heartbeat_message, reversed(heartbeat_messages)))
        )

            f.write(doc)

    def handle_heartbeat(message):
        log.info('Handling heartbeat')

        # Heartbeat messages as per msgflo participant discovery protocol
        d = message.payload.decode('utf8')
        m = json.loads(d)

        device = m['payload']['role']
        t = time.time()
        log.debug('saw device {} at {}'.format(device, t))
        m['time_received'] = t

        # Persist so we can query for device status
        if len(heartbeat_messages) == 100:
            # Remove the oldest to make space
            _oldest = heartbeat_messages.pop(0)
        heartbeat_messages.append(m)

    def mqtt_connected(client, u, f, rc):
        log.info('MQTT connected')

        mqtt_client.subscribe("fbp")

    def mqtt_disconnected(client, u, rc):
        log.info('MQTT disconnected: {}'.format(rc))
        # client automatically handles reconnect

    def mqtt_message_received(client, u, message):
        try:
            if message.topic == 'fbp':
                handle_heartbeat(message)
            else:
                raise ValueError("Unknown MQTT topic: {}".format(message.topic))
        except Exception as e:
            log.exception('Failed to handle MQTT message %: %s'.format(message.topic, message.payload))

    mqtt_client, host, port = create_mqtt_client(broker_url)
    mqtt_client.on_connect = mqtt_connected
    mqtt_client.on_disconnect = mqtt_disconnected
    mqtt_client.on_message = mqtt_message_received

    mqtt_client.connect(host, port)

    def check_heartbeats():
        previous_check = time.time() - check_interval
        devices = seen_since(heartbeat_messages, previous_check)
        if not 'display0' in devices:
            log.info("Missing heartbeat for 'display0'. Seen: {}".format(devices))

    def fetch_and_publish():
        log.info('Fetching new wind data')

        nonlocal wind_speed
        nonlocal wind_speed_updated_at_fmt
        nonlocal wind_speed_updated_at_time

        wind_speed = windspeed_ukenergy()
        wind_speed_updated_at_fmt = datetime.datetime.now().replace(microsecond=0).isoformat()
        wind_speed_updated_at_time = time.time()

        mqtt_client.publish('display/display0/windspeed', wind_speed, retain=True)
        log.info('New windspeed is {}'.format(wind_speed))

    def fetcher():
        while running:
            try:
                fetch_and_publish()
                check_heartbeats()
            except Exception as e:
                log.exception('Failed to fetch data: {}'.format(str(e)))
            gevent.sleep(check_interval)
        if done_cb:
            done_cb()

    fetch_and_publish()
    gevent.spawn(fetcher)

    def status_page_writer():
        while running:
            try:
                write_status_page()
            except Exception as e:
                log.exception('Failed to write status page: {}'.format(str(e)))
            gevent.sleep(status_interval)

    write_status_page()
    gevent.spawn(status_page_writer)

    def shutdown():
        running = False
        mqtt_client.disconnect()

    return shutdown

def main():
    broker_url = os.environ.get('BROKER_URL', 'mqtt://localhost')
    fetch_interval = float(os.environ.get('FETCH_INTERVAL', 15*60))

    done = gevent.event.AsyncResult()
    app = setup_app(broker_url, check_interval=fetch_interval, done_cb=lambda x: done.set(None))
    log.info('Fetcher running')

    done.get(timeout=None) # wait forever

if __name__ == '__main__':
    main()

