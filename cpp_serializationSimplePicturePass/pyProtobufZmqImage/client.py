import zmq
from objectInfos_pb2 import objectInfos
import cv2
import numpy as np

context = zmq.Context()

#  Socket to talk to server
print("Connecting to  server...")
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5555")

# read image
img = cv2.imread('wall1.jpg',0)

#encode
img_encode = cv2.imencode('.bmp',img)[1]
str_encode = img_encode.tostring()

#send
socket.send(str_encode)

#  Get the reply.
message = socket.recv()

stObjectInfos = objectInfos()
stObjectInfos.ParseFromString(message)
for info in stObjectInfos.infos:
    print(info.objClass, "" , info.score,  "" , info.tlx,  "" , info.tly,  "" , info.brx,  "" , info.bry )