
import gevent
import numpy
from PyMata.pymata import PyMata

def toggle(board):
    board.digital_write(2, False)
    gevent.sleep(2.0)
    board.digital_write(2, True)
    gevent.sleep(2.0)
    board.digital_write(2, False)
    gevent.sleep(2.0)
    board.digital_write(2, True)

def main():
    port = '/dev/ttyACM0'

    base = 0.6
    gust = numpy.sin(numpy.linspace(0, 2*3.14, 100))
    wind = base + (0.3)*gust
    print(wind)

    board = PyMata(port, verbose=True)

    FAN_PIN = 3
    GAUGE_PIN = 6
    POTENTIOMETER_ANALOG_PIN = 0

    board.set_pin_mode(13, board.OUTPUT, board.DIGITAL)
    board.set_pin_mode(FAN_PIN, board.PWM, board.ANALOG)
    board.servo_config(GAUGE_PIN)
    board.set_pin_mode(POTENTIOMETER_ANALOG_PIN, board.INPUT, board.ANALOG)

    for _ in range(0, 100):

        for v in list(wind):
            print(v)

            pot1024 = board.analog_read(POTENTIOMETER_ANALOG_PIN)
            pot = (1.0 / 1024.0) * pot1024

            v_scaled = pot * v

            gauge_degrees = 180 - int(180.0 * v_scaled)
            board.analog_write(GAUGE_PIN, gauge_degrees)

            fan_duty = int(255 * v_scaled)
            board.analog_write(FAN_PIN, fan_duty)

            gevent.sleep(0.025)

    #board.set_pin_mode(PUSH_BUTTON, board.INPUT, board.DIGITAL, cb_push_button)
    #board.set_pin_mode(POTENTIOMETER, board.INPUT, board.ANALOG, cb_potentiometer)


if __name__ == '__main__':
    main()



