import zmq
import cv2
import numpy as np
import time

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:6666")

while True:
    #  Wait for next request from client
    message = socket.recv()

    # decode
    nparr = np.fromstring(message, np.uint8)
    img_decode = cv2.imdecode(nparr, cv2.IMREAD_GRAYSCALE)

    #show
    cv2.imshow("img_decode",img_decode)
    cv2.waitKey()

    #  Send reply back to client
    socket.send_string("Reply")