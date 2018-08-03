import zmq
import cv2
import numpy as np

context = zmq.Context()

#  Socket to talk to server
print("Connecting to hello world server...")
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5555")

img = cv2.imread('wall1.jpg',0)
img_encode = cv2.imencode('.bmp',img)[1]
print(type(img_encode))
#data_encode = np.array(img_encode)
#print(type(data_encode))
str_encode = img_encode.tostring()
print(type(str_encode))
socket.send(str_encode)

#  Get the reply.
message = socket.recv()
print("Received reply ", "[", message, "]")