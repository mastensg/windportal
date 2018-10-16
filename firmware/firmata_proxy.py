import gevent
import numpy
from PyMata.pymata import PyMata

import zmq

LED_PIN = 13
FAN_PIN = 3
GAUGE_PIN = 6
POTENTIOMETER_ANALOG_PIN = 0


def main():
    the_zmq_context = zmq.Context()

    the_zmq_publisher = the_zmq_context.socket(zmq.PUB)
    the_zmq_publisher.bind("ipc://zmq_ipc")

    the_zmq_subscriber = the_zmq_context.socket(zmq.SUB)
    the_zmq_subscriber.connect("ipc://zmq_ipc")
    the_zmq_subscriber.setsockopt(zmq.SUBSCRIBE, b"")

    while True:
        # process all messages if any
        while True:
            socks = dict(the_zmq_poller.poll(10))
            #print(len(socks))
            if 0 == len(socks):
                break

            if the_zmq_subscriber in socks:
                topic, message = the_zmq_subscriber.recv_string().split()

                if "gauge" == topic:
                    print("gauge {}".format(message))

        pot1024 = 512
        pot = (1.0 / 1024.0) * pot1024
        print("pot is", pot)
        the_zmq_publisher.send_string("potentiometer {}".format(pot))
        gevent.sleep(0.025)

    port = '/dev/ttyACM0'

    board = PyMata(port, verbose=True)

    board.set_pin_mode(LED_PIN, board.OUTPUT, board.DIGITAL)
    board.set_pin_mode(FAN_PIN, board.PWM, board.ANALOG)
    board.servo_config(GAUGE_PIN)
    board.set_pin_mode(POTENTIOMETER_ANALOG_PIN, board.INPUT, board.ANALOG)

    while True:
        pot1024 = board.analog_read(POTENTIOMETER_ANALOG_PIN)
        pot = (1.0 / 1024.0) * pot1024
        print("pot is", pot)
        the_zmq_publisher.send_string("potentiometer {}".format(pot))

        #gauge_degrees = 180 - int(180.0 * v_scaled)
        #board.analog_write(GAUGE_PIN, gauge_degrees)

        #fan_duty = int(255 * v_scaled)
        #board.analog_write(FAN_PIN, fan_duty)

        gevent.sleep(0.025)


if __name__ == '__main__':
    main()
