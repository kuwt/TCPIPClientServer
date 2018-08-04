import zmq
import cv2
import numpy as np

context = zmq.Context()

#  Socket to talk to server
print("Connecting to server...")
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:6666")

# read
img = cv2.imread('wall1.jpg',0)

# write
bufferPath = r"E:\backup\temp.bmp"
cv2.imwrite(bufferPath,img)

#send path
data = bufferPath
str_encode = data.encode(encoding='UTF-8')
socket.send(str_encode)

#  Get the reply.
message = socket.recv()
print("Received reply ", "[", message, "]")