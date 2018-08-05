import zmq
from foo_pb2 import Foo

context = zmq.Context()

#  Socket to talk to server
print("Connecting to hello world server...")
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5555")

foo = Foo()
foo.id = 6
foo.bar = "sdd"
str = foo.SerializeToString()
socket.send(str)

message = socket.recv()
