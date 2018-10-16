import gevent
import numpy
from PyMata.pymata import PyMata


def main():
    port = '/dev/ttyACM0'

    base = 0.3
    gust = numpy.sin(numpy.linspace(0, 2 * 3.14, 100))
    wind = base + (0.1) * gust
    print(wind)

    board = PyMata(port, verbose=True)

    GAUGE_PIN = 6
    POTENTIOMETER_ANALOG_PIN = 0

    board.set_pin_mode(13, board.OUTPUT, board.DIGITAL)
    #board.set_pin_mode(FAN_PIN, board.PWM, board.ANALOG)
    board.servo_config(GAUGE_PIN)
    board.set_pin_mode(POTENTIOMETER_ANALOG_PIN, board.INPUT, board.ANALOG)

    while True:
        pot = board.analog_read(POTENTIOMETER_ANALOG_PIN)
        deg = 180 - int(180.0 * (pot / 1023.0))
        print("{:4} => {:3}".format(pot, deg))

        board.analog_write(GAUGE_PIN, deg)

        gevent.sleep(0.025)

    for _ in range(0, 10):

        for v in list(wind):
            print(v)
            board.analog_write(FAN_PIN, (int)(v * 255))
            gevent.sleep(0.025)

    #board.set_pin_mode(PUSH_BUTTON, board.INPUT, board.DIGITAL, cb_push_button)


if __name__ == '__main__':
    main()
