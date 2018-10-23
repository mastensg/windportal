
import os

import pytest
import gevent

import wp_web

BROKER = os.environ.get('BROKER_URL', 'mqtt://localhost')

def test_fetch_windspeed():
    wind = wp_web.windspeed_ukenergy()
    assert 1.0 < wind < 25.0

@pytest.fixture()
def mqtt_client():
    mqtt_client, host, port = wp_web.create_mqtt_client(BROKER)
    timeout = 2
    r = mqtt_client.connect(host, port, timeout)

    gevent.sleep(0.5)
    yield mqtt_client
    mqtt_client.disconnect()

@pytest.fixture()
def fetcher():
    shutdown_app = wp_web.setup_app(broker_url=BROKER, check_interval=0.5)
    yield None
    shutdown_app()

def test_fetcher_gets_windspeed(fetcher, mqtt_client):
    topic = 'display/display0/windspeed'
    mqtt_client.subscribe(topic)

    reset_speed = 6.666
    mqtt_client.publish(topic, reset_speed)

    result = gevent.event.AsyncResult()
    def on_message(client, u, message):
        assert message.topic == topic
        s = message.payload.decode('utf8')
        wind_speed = float(s)
        is_reset = abs(wind_speed - reset_speed) < 0.001
        if not is_reset:
            result.set(wind_speed)

    mqtt_client.on_message = on_message
    wind = result.get(timeout=3.0)
    assert 1.0 < wind < 25.0


