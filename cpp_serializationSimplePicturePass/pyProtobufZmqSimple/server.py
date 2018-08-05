import zmq
import time
from foo_pb2 import Foo
context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")


while True:
    message = socket.recv()
    foo = Foo()
    foo.ParseFromString(message)
    print("Got foo with id={0} and bar={1}".format(foo.id, foo.bar))

    #  Send reply back to client
    socket.send_string("reply")