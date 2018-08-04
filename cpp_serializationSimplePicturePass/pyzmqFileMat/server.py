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
    
    #decode
    str = message.decode("utf-8") 
   
    #read
    img = cv2.imread(str,0)
    
    #show
    cv2.imshow("img",img)
    cv2.waitKey()
   
    #  Send reply back to client
    socket.send_string("Reply")