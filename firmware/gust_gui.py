import matplotlib as mpl
import numpy as np
import sys
import tkinter as tk
import time
import matplotlib.backends.tkagg as tkagg
from matplotlib.backends.backend_agg import FigureCanvasAgg

import firmata_ipc

the_ipc_session = firmata_ipc.Session()

def draw_figure(canvas, figure, loc=(0, 0)):
    """ Draw a matplotlib figure onto a Tk canvas

    loc: location of top-left corner of figure on canvas in pixels.
    Inspired by matplotlib source: lib/matplotlib/backends/backend_tkagg.py
    """
    figure_canvas_agg = FigureCanvasAgg(figure)
    figure_canvas_agg.draw()
    figure_x, figure_y, figure_w, figure_h = figure.bbox.bounds
    figure_w, figure_h = int(figure_w), int(figure_h)
    photo = tk.PhotoImage(master=canvas, width=figure_w, height=figure_h)

    # Position: convert from top-left anchor to center anchor
    canvas.create_image(
        loc[0] + figure_w / 2, loc[1] + figure_h / 2, image=photo)

    # Unfortunately, there's no accessor for the pointer to the native renderer
    tkagg.blit(photo, figure_canvas_agg.get_renderer()._renderer, colormode=2)

    # Return a handle which contains a reference to the photo object
    # which must be kept live or else the picture disappears
    return photo


the_window = tk.Tk()
the_window.title("A figure in a canvas")

the_window.bind('<Escape>', sys.exit)

the_canvas = tk.Canvas(the_window, width=300, height=200)
the_canvas.pack()

the_fig_photo = None


def wind_speed_change(s_str):
    s = float(s_str)

    X = np.linspace(0, (0.01 * s) * 2 * np.pi, 50)
    Y = np.sin(X)

    # Create the figure we desire to add to an existing canvas
    fig = mpl.figure.Figure(figsize=(2, 1))
    ax = fig.add_axes([0, 0, 1, 1])
    ax.plot(X, Y)

    the_canvas.delete("all")

    global the_fig_photo
    the_fig_photo = draw_figure(the_canvas, fig, loc=(200, 200))


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


def print_wind_speed():
    #print(the_wind_speed.get())
    the_window.after(300, print_wind_speed)


print_wind_speed()

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


def ipc_recv():
    published_values = the_ipc_session.recv()
    for topic, value in published_values.items():
        if "potentiometer" == topic:
            the_potentiometer.set(value)

    the_window.after(25, ipc_recv)


ipc_recv()

tk.mainloop()
