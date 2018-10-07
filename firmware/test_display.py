
import display

import pytest
import msgflo
import gevent

import os.path


# Helper for running one iteration of next_state()
def run_next(state, inputs):
    current = display.State(**state)
    inputs = display.Inputs(**inputs)
    next = display.next_state(current, inputs)
    return next

# State calculation tests
def test_no_wind_no_fan():
    state = run_next({}, dict(time=0.0,windspeed=0.0))
    assert state.fan_speed == 0.0




def create_mqtt_client(broker_url):
    import paho.mqtt.client as mqtt
    from urllib.parse import urlparse

    broker_info = urlparse(broker_url)

    client = mqtt.Client()
    if broker_info.username:
        client.username_pw_set(broker_info.username, broker_info.password)

    client.reconnect_delay_set(min_delay=1, max_delay=2*60)

    host = broker_info.hostname
    default_port = 1883
    if broker_info.scheme == 'mqtts':
        default_port = 8883
        client.tls_set()
    port = broker_info.port or default_port

    # XXX loop() does not handle reconnects, have to use loop_start() or loop_forever() 
    client.loop_start()

    return client, host, port

@pytest.fixture(scope="module")
def mqtt_client():
    broker_url = os.environ.get('MSGFLO_BROKER', 'mqtt://localhost')
    mqtt_client, host, port = create_mqtt_client(broker_url)
    timeout = 5
    mqtt_client.connect(host, port, timeout)
    yield mqtt_client
    mqtt_client.disconnect()

@pytest.fixture()
def participant():
    role = 'testdisplay/0'
    participant = display.Participant(role)
    engine = msgflo.run([participant])
    yield participant
    engine._client = None # disconnect

# MQTT / tests
def test_mqtt_windspeed_update(participant, mqtt_client):
    if os.path.exists(participant.windspeed_file):
        os.remove(participant.windspeed_file)

    role = participant.definition['role']
    topic = role + '/windspeed'
    assert 'testdisplay/0' in topic
    assert participant.inputs.windspeed == 0.0

    # Send new windspeed
    mqtt_client.publish(topic, '13.44')
    gevent.sleep(0.5)

    # FIXME: check get response
    # check was written to disk
    assert participant.inputs.windspeed == 13.44
    assert os.path.exists(participant.windspeed_file)
    
def test_windspeed_load_disk():
    participant = display.Participant('testdisplay/1')
    if os.path.exists(participant.windspeed_file):
        os.remove(participant.windspeed_file)

    participant.recalculate_state()
    assert participant.inputs.windspeed == 0.0

    with open(participant.windspeed_file, 'w') as f:
        f.write('12.12')
    participant.recalculate_state()
    assert participant.inputs.windspeed == 12.12
