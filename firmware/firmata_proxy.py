
import gevent
from PyMata.pymata import PyMata

import firmata_ipc

LED_PIN = 13
FAN_PIN = 3
GAUGE_PIN = 6
POTENTIOMETER_ANALOG_PIN = 0


def main():
    the_ipc_session = firmata_ipc.Session(bind=True)

    port = '/dev/ttyACM0'

    board = PyMata(port, verbose=True)

    board.set_pin_mode(LED_PIN, board.OUTPUT, board.DIGITAL)
    board.set_pin_mode(FAN_PIN, board.PWM, board.ANALOG)
    board.servo_config(GAUGE_PIN)
    board.set_pin_mode(POTENTIOMETER_ANALOG_PIN, board.INPUT, board.ANALOG)

    while True:
        published_values = the_ipc_session.recv()
        for topic, value in published_values.items():
            if "fan_duty" == topic:
                fan_duty255 = int(255 * value)
                board.analog_write(FAN_PIN, fan_duty255)
            elif "gauge_degrees" == topic:
                board.analog_write(GAUGE_PIN, 180 - value)
            elif "led" == topic:
                board.digital_write(LED_PIN, value)

        pot1024 = board.analog_read(POTENTIOMETER_ANALOG_PIN)
        #pot1024=999
        pot = (1.0 / 1024.0) * pot1024
        the_ipc_session.send("potentiometer", pot)

        print(".", end="", flush=True)
        gevent.sleep(0.025)


if __name__ == '__main__':
    main()
