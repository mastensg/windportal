
import gevent
from pyfirmata import Arduino, util


def main():
    port = '/dev/ttyUSB0'
    board = Arduino(port)

    print('on')
    board.digital[13].write(1)
    gevent.sleep(3.5)
    print('off')
    board.digital[13].write(0)
    gevent.sleep(3.5)

if __name__ == '__main__':
    main()



