
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

    board = PyMata(port, verbose=True)

    FAN_PIN = 3

    board.set_pin_mode(13, board.OUTPUT, board.DIGITAL)
    board.set_pin_mode(FAN_PIN, board.PWM, board.ANALOG)

    d = numpy.sin(numpy.linspace(0, 3.14, 100))
    print('d', d)

    for _ in range(0, 10):
        for v in list(d):
            print(v)
            board.analog_write(FAN_PIN, (int)(v*255))
            gevent.sleep(0.01)

    #board.set_pin_mode(PUSH_BUTTON, board.INPUT, board.DIGITAL, cb_push_button)
    #board.set_pin_mode(POTENTIOMETER, board.INPUT, board.ANALOG, cb_potentiometer)


if __name__ == '__main__':
    main()



