#!/usr/bin/env python3

import os

import gevent
from PyMata.pymata import PyMata

import wp_ipc

LED_PIN = 13
FAN_PIN = 3
GAUGE_PIN = 6
POTENTIOMETER_ANALOG_PIN = 0
BUTTON_PIN = 2

def main():
    the_ipc_session = wp_ipc.Session()

    port = os.environ.get('FIRMATA_PORT', '/dev/ttyACM0')

    board = PyMata(port, verbose=True, bluetooth=False)

    board.set_pin_mode(LED_PIN, board.OUTPUT, board.DIGITAL)
    board.set_pin_mode(BUTTON_PIN, board.PULLUP, board.DIGITAL)
    board.set_pin_mode(FAN_PIN, board.PWM, board.ANALOG)
    board.servo_config(GAUGE_PIN)
    board.set_pin_mode(POTENTIOMETER_ANALOG_PIN, board.INPUT, board.ANALOG)

    while True:
        published_values = the_ipc_session.recv()
        for topic, value in published_values.items():
            if "fan_duty" == topic:
                fan_duty255 = int(255 * value)
                if fan_duty255 > 255:
                    fan_duty255 = 255
                if fan_duty255 < 0:
                    fan_duty255 = 0
                board.analog_write(FAN_PIN, fan_duty255)
            elif "gauge_degrees" == topic:
                board.analog_write(GAUGE_PIN, 180 - value)
            elif "led" == topic:
                board.digital_write(LED_PIN, value)

        pot1024 = board.analog_read(POTENTIOMETER_ANALOG_PIN)
        pot = (1.0 / 1024.0) * pot1024

        button_pressed = not bool(board.digital_read(BUTTON_PIN))

        the_ipc_session.send("potentiometer", pot)
        the_ipc_session.send("button", button_pressed)

        gevent.sleep(0.100)


if __name__ == '__main__':
    main()
