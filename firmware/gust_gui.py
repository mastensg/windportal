#!/usr/bin/env python3

import sys
import tkinter as tk
import time
import copy

import wp_ipc
import wp_gust


##############################################################################


def plot():
    import matplotlib as mpl
    import matplotlib.backends.tkagg as tkagg
    from matplotlib.backends.backend_agg import FigureCanvasAgg
    from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg,
                                                   NavigationToolbar2Tk)
    import pandas as pd

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
        'perturbation_amplitude': the_perturbation_amplitude,
        'wind_speed': the_wind_speed,
        'scale': the_potentiometer,
    }
    outputs = {
        'fan_duty': the_fan_duty,
        'perturbation': the_perturbation,
    }

    return the_window, inputs, outputs

##############################################################################

def update_inputs_gui(inputs, widgets):
    for name, widget in widgets.items():
        inputs.__dict__[name] = float(widget.get())

def set_outputs_gui(widgets, state):
    for name, widget in widgets.items():
        v = state.__dict__[name]
        widgets[name].set(v)

def main():
    loop_interval = 100
    state = wp_gust.State()
    inputs = wp_gust.Inputs()

    ipc_session = wp_ipc.Session()
    window, input_widgets, output_widgets = setup_gui()

    def loop():
        nonlocal state
        inputs.__dict__['time'] = time.time()
        wp_gust.update_inputs_ipc(inputs, ipc_session)
        update_inputs_gui(inputs, input_widgets)

        state = wp_gust.next_state(state, inputs)

        set_outputs_gui(output_widgets, state)
        wp_gust.set_outputs_ipc(ipc_session, state)

        window.after(loop_interval, loop)

    loop()

    tk.mainloop()

if __name__ == '__main__':
    main()


