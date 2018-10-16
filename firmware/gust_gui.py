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

the_period = 50
the_noise = np.random.normal(0.0, 0.1, the_period)
the_noise_series = pd.Series(the_noise)


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
    to=100.0,
    resolution=0.1,
    orient=tk.HORIZONTAL,
    label="wind speed",
    length=300,
    command=wind_speed_change)
the_wind_speed.pack()
the_wind_speed.set(20.0)


def gauge_change(s_str):
    s = int(float(s_str))
    the_ipc_session.send("gauge_degrees", s)


the_gauge = tk.Scale(
    the_window,
    from_=0.0,
    to=180.0,
    resolution=0.1,
    orient=tk.HORIZONTAL,
    label="gauge degrees",
    length=300,
    command=gauge_change)
the_gauge.pack()


def potentiometer_change(s_str):
    s = int(180.0 * float(s_str))
    #global the_gauge
    #the_gauge.set(s)
    #the_ipc_session.send("gauge_degrees", s)


the_potentiometer = tk.Scale(
    the_window,
    from_=0.0,
    to=1.0,
    resolution=0.01,
    orient=tk.HORIZONTAL,
    label="potentiometer",
    length=300,
    command=potentiometer_change)
the_potentiometer.pack()


def ipc_recv():
    published_values = the_ipc_session.recv()
    for topic, value in published_values.items():
        if "potentiometer" == topic:
            the_potentiometer.set(value)

    the_window.after(25, ipc_recv)


ipc_recv()


def fan_duty_change(s_str):
    s = float(s_str)
    the_ipc_session.send("fan_duty", s)


the_fan_duty = tk.Scale(
    the_window,
    from_=0.0,
    to=1.0,
    resolution=0.01,
    orient=tk.HORIZONTAL,
    label="fan duty",
    length=300,
    command=fan_duty_change)
the_fan_duty.pack()

the_noise_phase = 0


def perturb_fan_duty():
    global the_noise_phase

    n = the_noise[the_noise_phase]
    print(n)

    #the_fan_duty.set(n)
    the_gauge.set(n)

    the_noise_phase = the_noise_phase + 1 if the_noise_phase < the_period - 1 else 0

    the_window.after(500, perturb_fan_duty)


perturb_fan_duty()

tk.mainloop()
