import sys
import tkinter as tk
import time

import numpy as np
import matplotlib as mpl
import matplotlib.backends.tkagg as tkagg
from matplotlib.backends.backend_agg import FigureCanvasAgg
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg,
                                               NavigationToolbar2Tk)

import pandas as pd

import firmata_ipc

the_ipc_session = firmata_ipc.Session()

the_window = tk.Tk()
the_window.title("A figure in a canvas")

the_window.bind('<Escape>', sys.exit)

the_canvas = tk.Canvas(the_window, width=300, height=200)
the_canvas.pack()

the_fig_photo = None

the_period = 200
the_noise = np.random.normal(0.0, 1.0, the_period)
the_noise_series = pd.Series(the_noise)

##############################################################################


def wind_speed_change(s_str):
    s = float(s_str)

    fig = mpl.figure.Figure(figsize=(3, 2))

    ax = fig.add_subplot(111)

    figure_canvas_agg = FigureCanvasAgg(fig)

    dplt = the_noise_series.plot(ax=ax)

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


the_wind_speed = tk.Scale(
    the_window,
    from_=0.0,
    to=40.0,
    resolution=0.1,
    orient=tk.HORIZONTAL,
    label="wind speed",
    length=300,
    command=wind_speed_change)
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

the_fan_flow_response_up = tk.Scale(
    the_window,
    from_=0.00,
    to=0.1,
    resolution=0.001,
    orient=tk.HORIZONTAL,
    label="fan flow response up",
    length=300,
    command=None)
the_fan_flow_response_up.pack()
the_fan_flow_response_up.set(0.002)

the_fan_flow_response_down = tk.Scale(
    the_window,
    from_=0.00,
    to=0.1,
    resolution=0.001,
    orient=tk.HORIZONTAL,
    label="fan flow response down",
    length=300,
    command=None)
the_fan_flow_response_down.pack()
the_fan_flow_response_down.set(0.001)

the_fan_flow = tk.Scale(
    the_window,
    from_=0.0,
    to=1.0,
    resolution=0.001,
    orient=tk.HORIZONTAL,
    label="fan flow",
    length=300,
    command=None)
the_fan_flow.pack()

##############################################################################


def ipc_recv():
    published_values = the_ipc_session.recv()
    for topic, value in published_values.items():
        if "potentiometer" == topic:
            the_potentiometer.set(value)

    the_window.after(25, ipc_recv)


ipc_recv()


def ipc_send():
    the_ipc_session.send("fan_duty", float(the_fan_duty.get()))
    the_window.after(25, ipc_recv)


ipc_send()

the_noise_phase = 0


def update_perturbation():
    perturbation_amplitude = the_perturbation_amplitude.get()

    global the_noise_phase

    n = the_noise[the_noise_phase]
    p = perturbation_amplitude * n

    the_perturbation.set(p)

    the_noise_phase = the_noise_phase + 1 if the_noise_phase < the_period - 1 else 0

    the_window.after(the_perturbation_period.get(), update_perturbation)


update_perturbation()


def update_fan_duty():
    wind_speed = the_wind_speed.get()
    potentiometer = the_potentiometer.get()
    perturbation = the_perturbation.get()

    duty = (1 / 40) * wind_speed * potentiometer + perturbation

    the_fan_duty.set(duty)

    the_window.after(50, update_fan_duty)


update_fan_duty()


def update_fan_flow():
    d = the_fan_duty.get()
    f = the_fan_flow.get()

    if f < d:
        f += the_fan_flow_response_up.get()
    elif d < f:
        f -= the_fan_flow_response_down.get()

    the_fan_flow.set(f)

    the_window.after(50, update_fan_flow)


update_fan_flow()

tk.mainloop()
