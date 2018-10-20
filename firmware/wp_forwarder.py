#!/usr/bin/env python3

import zmq

import wp_ipc


def main():
    context = zmq.Context(1)

    sub = context.socket(zmq.SUB)
    sub.bind(wp_ipc.PUB_URL)

    sub.setsockopt(zmq.SUBSCRIBE, b"")

    pub = context.socket(zmq.PUB)
    pub.bind(wp_ipc.SUB_URL)

    zmq.device(zmq.FORWARDER, sub, pub)


if __name__ == "__main__":
    main()
