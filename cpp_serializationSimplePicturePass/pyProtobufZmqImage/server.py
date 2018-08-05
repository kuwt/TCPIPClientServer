import zmq
import time
from objectInfos_pb2 import objectInfos
import cv2
import numpy as np

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")


while True:
    message = socket.recv()
    if (message):
        # decode
        nparr = np.fromstring(message, np.uint8)
        img_decode = cv2.imdecode(nparr, cv2.IMREAD_GRAYSCALE)

        # show
        cv2.imshow("img_decode", img_decode)
        cv2.waitKey()

    #  Send reply back to client
    stObjectInfos = objectInfos()
    info = stObjectInfos.infos.add()
    info.objClass = 0
    info.score = 1
    info.tlx = 1
    info.tly = 2
    info.brx = 11
    info.bry = 15
    info = stObjectInfos.infos.add()
    info.objClass = 0
    info.score = 3
    info.tlx = 4
    info.tly = 5
    info.brx = 123
    info.bry = 333

    str = stObjectInfos.SerializeToString()
    socket.send(str)