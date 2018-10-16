import json

import zmq

PUB_URL = "ipc:///tmp/firmata_pub"
SUB_URL = "ipc:///tmp/firmata_sub"


class Session:
    def __init__(self, bind=False):
        self.context = zmq.Context()

        self.sub = self.context.socket(zmq.SUB)
        if bind:
            self.sub.bind(SUB_URL)
        else:
            self.sub.connect(PUB_URL)
        self.sub.setsockopt(zmq.SUBSCRIBE, b"")

        self.poller = zmq.Poller()
        self.poller.register(self.sub, zmq.POLLIN)

        self.pub = self.context.socket(zmq.PUB)
        if bind:
            self.pub.bind(PUB_URL)
        else:
            self.pub.connect(SUB_URL)

    def recv(self, timeout_ms=10):
        values = {}

        while True:
            socks = dict(self.poller.poll(timeout_ms))
            if 0 == len(socks):
                break

            if self.sub in socks:
                topic, message = self.sub.recv_string().split()
                values[topic] = json.loads(message)

        return values

    def send(self, topic, value):
        self.pub.send_string("{} {}".format(topic, json.dumps(value)))


def main():
    """ Print all published messages.
    """
    s = Session()

    while True:
        r = s.recv()
        if {} != r:
            print(r)


if __name__ == '__main__':
    main()
