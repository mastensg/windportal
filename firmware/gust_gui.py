#!/usr/bin/env python3

import sys
import tkinter as tk
import time
import copy

import numpy as np
import matplotlib as mpl
import matplotlib.backends.tkagg as tkagg
from matplotlib.backends.backend_agg import FigureCanvasAgg
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg,
                                               NavigationToolbar2Tk)

import pandas as pd

import wp_ipc


##############################################################################


# Poor-mans dataclasses, since we don't have Python 3.7
class Inputs(): 
  def __init__(self,
        # Actual inputs
        wind_speed  : float = 0.0,
        scale : float = 1.0,

        # Parameters
        perturbation_period : float = 1000.0, # milliseconds
        perturbation_amplitude: float = 0.1,
        noise_period : int = 200,
        
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
        state.noise = np.random.normal(0.0, 1.0, inputs.noise_period)

    if inputs.time >= current.next_perturbation_update:
        n = state.noise[state.noise_phase]
        p = inputs.perturbation_amplitude * n

        state.noise_phase = state.noise_phase + 1 if state.noise_phase < inputs.noise_period - 1 else 0
        state.perturbation = p
        state.next_perturbation_update = inputs.time + (inputs.perturbation_period/1000.0)

    state.fan_duty = (1 / 40) * inputs.wind_speed * inputs.scale + state.perturbation

    return state




def plot():
    fig = mpl.figure.Figure(figsize=(3, 2))

    ax = fig.add_subplot(111)

    figure_canvas_agg = FigureCanvasAgg(fig)

    series = pandas.Series(data)
    dplt = series.plot(ax=ax)

    figure_canvas_agg.draw()
    figure_x, figure_y, figure_w, figure_h = fig.bbox.bounds
    figure_w, figure_h = int(figure_w), int(figure_h)
    global the_fig_photo
    the_fig_photo = tk.PhotoImage(
        master=the_canvas, width=figure_w, height=figure_h)

    # Position: convert from top-left anchor to center anchor
    loc = (0, 0)
    the_canvas.delete("all")
    the_canvas.create_image(
        loc[0] + figure_w / 2, loc[1] + figure_h / 2, image=the_fig_photo)

    tkagg.blit(
        the_fig_photo, figure_canvas_agg.get_renderer()._renderer, colormode=2)

    return the_fig_photo # XXX: has to be held


def setup_gui():
    the_window = tk.Tk()
    the_window.title("A figure in a canvas")

    the_window.bind('<Escape>', sys.exit)

    the_canvas = tk.Canvas(the_window, width=300, height=200)
    the_canvas.pack()

    the_fig_photo = None


    the_wind_speed = tk.Scale(
        the_window,
        from_=0.0,
        to=40.0,
        resolution=0.1,
        orient=tk.HORIZONTAL,
        label="wind speed",
        length=300,
        command=None)
    the_wind_speed.pack()
    the_wind_speed.set(20.0)

    the_potentiometer = tk.Scale(
        the_window,
        from_=0.0,
        to=1.0,
        resolution=0.01,
        orient=tk.HORIZONTAL,
        label="potentiometer",
        length=300,
        command=None)
    the_potentiometer.pack()
    the_potentiometer.set(0.5)

    the_perturbation_period = tk.Scale(
        the_window,
        from_=10,
        to=10000,
        resolution=1,
        orient=tk.HORIZONTAL,
        label="perturbation period (ms)",
        length=300,
        command=None)
    the_perturbation_period.pack()
    the_perturbation_period.set(1000)

    the_perturbation_amplitude = tk.Scale(
        the_window,
        from_=0.0,
        to=1.0,
        resolution=0.001,
        orient=tk.HORIZONTAL,
        label="perturbation amplitude",
        length=300,
        command=None)
    the_perturbation_amplitude.pack()
    the_perturbation_amplitude.set(0.1)

    the_perturbation = tk.Scale(
        the_window,
        from_=-1.0,
        to=1.0,
        resolution=0.01,
        orient=tk.HORIZONTAL,
        label="perturbation",
        length=300,
        command=None)
    the_perturbation.pack()

    the_fan_duty = tk.Scale(
        the_window,
        from_=0.0,
        to=1.0,
        resolution=0.01,
        orient=tk.HORIZONTAL,
        label="fan duty",
        length=300,
        command=None)
    the_fan_duty.pack()

    tk.Label(the_window, text="\nsimulation\n").pack()

    #the_fan_duty

    inputs = {
        'perturbation_period': the_perturbation_period,
        'wind_speed': the_wind_speed,
        'scale': the_potentiometer,
    }
    outputs = {
        'fan_duty': the_fan_duty,
        'perturbation': the_perturbation,
    }

    return the_window, inputs, outputs

##############################################################################


def update_inputs_ipc(inputs, ipc_session):
    published_values = ipc_session.recv()
    for topic, value in published_values.items():
        if "potentiometer" == topic:
            inputs.__dict__['scale'] = value
        if "wind_speed" == topic:
            inputs.__dict__['wind_speed'] = value

def update_inputs_gui(inputs, widgets):
    for name, widget in widgets.items():
        inputs.__dict__[name] = float(widget.get())

def set_outputs_gui(widgets, state):
    for name, widget in widgets.items():
        v = state.__dict__[name]
        widgets[name].set(v)

def main():
    loop_interval = 100
    state = State()
    inputs = Inputs()

    ipc_session = wp_ipc.Session()
    window, input_widgets, output_widgets = setup_gui()

    def loop():
        nonlocal state
        inputs.__dict__['time'] = time.time()
        update_inputs_ipc(inputs, ipc_session)
        update_inputs_gui(inputs, input_widgets)

        state = next_state(state, inputs)

        s = copy.copy(state.__dict__)
        del s['noise']
        print('i', inputs.__dict__)
        print('s', s)

        set_outputs_gui(output_widgets, state) 
        ipc_session.send("fan_duty", state.fan_duty)

        window.after(loop_interval, loop)

    loop()

    tk.mainloop()

if __name__ == '__main__':
    main()


