
import wp_ipc

import gevent
import gevent.event

import sys
import time
import copy
import random


# Poor-mans dataclasses, since we don't have Python 3.7
class Inputs(): 
  def __init__(self,
        # Actual inputs
        wind_speed  : float = 0.0,
        scale : float = 1.0,
        user_button: bool = False,

        # Parameters
        perturbation_period : float = 1000.0, # milliseconds
        perturbation_amplitude: float = 0.1,
        noise_period : int = 200,
        idle_scale : float = 0.2,
        
        time : float = 0.0):

    attrs = locals()
    del attrs['self']
    self.__dict__.update(attrs)

class State():
  def __init__(self,
        fan_duty : float = 0.0,

        perturbation : float = 0.0,
        next_perturbation_update : float = 0.0,
        noise = None,
        noise_phase : int = 0 ):

    attrs = locals()
    del attrs['self']
    self.__dict__.update(attrs)


def next_state(current : State, inputs: Inputs):
    state = copy.copy(current)

    if state.noise is None:
        state.noise = [ random.normalvariate(0.0, 1.0) for _ in range(inputs.noise_period) ]

    if inputs.time >= current.next_perturbation_update:
        n = state.noise[state.noise_phase]
        p = inputs.perturbation_amplitude * n

        state.noise_phase = state.noise_phase + 1 if state.noise_phase < inputs.noise_period - 1 else 0
        state.perturbation = p
        state.next_perturbation_update = inputs.time + (inputs.perturbation_period/1000.0)

    user_scale = 1.0 if inputs.user_button else inputs.idle_scale

    state.fan_duty = (1 / 40) * user_scale * inputs.wind_speed * inputs.scale + state.perturbation

    return state

def update_inputs_ipc(inputs, ipc_session):
    published_values = ipc_session.recv()
    for topic, value in published_values.items():
        if "potentiometer" == topic:
            inputs.__dict__['scale'] = value
        if "button" == topic:
            inputs.__dict__['user_button'] = value
        if "wind_speed" == topic:
            inputs.__dict__['wind_speed'] = value

def set_outputs_ipc(ipc_session, state):
    ipc_session.send("fan_duty", state.fan_duty)


def main():
    loop_interval = 100
    state = State()
    inputs = Inputs()
    ipc_session = wp_ipc.Session()

    def loop():
        nonlocal state
        inputs.__dict__['time'] = time.time()
        update_inputs_ipc(inputs, ipc_session)

        state = next_state(state, inputs)

        s = copy.copy(state.__dict__)
        del s['noise']
        print('i', inputs.__dict__)
        print('s', s)

        set_outputs_ipc(ipc_session, state)

    print('main')
    result = gevent.event.AsyncResult()
    def run():
        while True:
            loop()
            gevent.sleep(loop_interval/1000)
        result.set('done')

    gevent.spawn(run)
    result.get(timeout=None) # wait forever


if __name__ == '__main__':
    main()
