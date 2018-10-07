
import gpio

import gevent.monkey
gevent.monkey.patch_all()

import gevent
import msgflo

import os
import sys
import time
import os.path
import json
import math
import copy
import random

import logging
logging.basicConfig()
log_level = os.environ.get('LOGLEVEL', 'info')
level = getattr(logging, log_level.upper())
log = logging.getLogger('display')
log.setLevel(level)


# Poor-mans dataclasses, since we don't have Python 3.7
class Inputs(): 
  def __init__(self,
        time : float, # seconds
        windspeed  : float, # meters/second
        mqtt_connected : bool = False):

    attrs = locals()
    del attrs['self']
    self.__dict__.update(attrs)

class State():
  def __init__(self,
        fan_speed : float = 0.0,
        connected_led : bool = False):

    attrs = locals()
    del attrs['self']
    self.__dict__.update(attrs)


def map_linear(x,in_low,in_high,out_low,out_high):
  return 0.0

#
def next_state(current : State, inputs : Inputs):
  i = inputs

  gust = map_linear(random.random(), 0.0, 1.0, -2.0, 3.0)
  fan_speed = map_linear(i.windspeed + gust, 0.0, 30.0, 0.0, 1.0)

  state = State(
    fan_speed = fan_speed 
  )

  return state


def set_outputs(state : State):
  print('TODO: realize outputs', state.__dict__)


participant_definition = {
  'component': 'windmachine/Display',
  'label': 'Demonstrates the power of the wind',
  'icon': 'clock-o',
  'inports': [
    { 'id': 'windspeed', 'type': 'number' },
  ],
  'outports': [
    { 'id': 'error', 'type': 'string' },
    { 'id': 'currentwindspeed', 'type': 'number' },
  ],
}

# Used for testing
class AlwaysErroringParticipant(msgflo.Participant):
  def __init__(self, role):
    d = copy.deepcopy(participant_definition)
    msgflo.Participant.__init__(self, d, role)

  def process(self, inport, msg):
    if inport == 'windspeed':
        self.send('error', "Updating windspeed always fails")
    else:
        self.send('error', 'Unknown port {}'.format(inport))
    self.ack(msg)

class Participant(msgflo.Participant):
  def __init__(self, role):
    d = copy.deepcopy(participant_definition)
    msgflo.Participant.__init__(self, d, role)

    self.windspeed_file = role.replace('/', '_')+'.windspeed.txt'

    self.state = State()
    self.inputs = Inputs(time=0, windspeed=0.0)

    gevent.Greenlet.spawn(self.loop)

  def process(self, inport, msg):
    self.recalculate_state(mqtt_windspeed=msg.data)
    self.ack(msg)

  def loop(self):
    while True:
      self.recalculate_state()
      gevent.sleep(0.1)

  def is_connected(self):
    return getattr(self, '_engine', None) and self._engine.connected

  def get_inputs(self) -> Inputs:
    windspeed = 0.0
    try:
      with open(self.windspeed_file, 'r') as f:
        windspeed = float(f.read())
    except FileNotFoundError:
      pass
    except ValueError as e:
      logging.warning("Error reading windspeed file, removing. " + str(e))
      os.remove(self.windspeed_file)

    inputs = Inputs(
        time=time.monotonic(),
        mqtt_connected=self.is_connected(),
        windspeed=windspeed,
    )
    return inputs

  def recalculate_state(self, mqtt_windspeed=None):

    if mqtt_windspeed:
      # Got new update, write it to disk
      with open(self.windspeed_file, 'w') as f:
        f.write(str(mqtt_windspeed))

      if self.is_connected():
        self.send('currentwindspeed', mqtt_windspeed) 

    inputs = self.get_inputs()
    next = next_state(copy.deepcopy(self.state), copy.deepcopy(inputs))
    set_outputs(next)

    state_changed = next != self.state
    if state_changed:
      entry = { 'inputs': inputs.__dict__, 'state': next.__dict__ }
      log.info(entry)

    self.state = next
    self.inputs = inputs

def main():
    role = os.environ.get('DISPLAY_PREFIX', 'display/X')
    if len(sys.argv) >= 2:
      role = sys.argv[1]
    p = msgflo.main(Participant, role=role)

if __name__ == '__main__':
    main()
