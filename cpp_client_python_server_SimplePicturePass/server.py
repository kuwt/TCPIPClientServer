import socket
import sys
import cv2
import numpy as np

ipAddress = "localhost"
portNumber = 8888

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the port
server_address = ('localhost', portNumber)
print(sys.stderr, 'starting up on %s port %s' % server_address)
sock.bind(server_address)
	
# Listen for incoming connections
sock.listen(1)

while True:
    # Wait for a connection
    print(sys.stderr, 'waiting for a connection')
    connection, client_address = sock.accept()
    try:
        print(sys.stderr, 'connection from', client_address)

        segmentSize = 1024
        ImageSrc = np.zeros((480,640,1), np.uint8)
        SegmentStartIdx = 0
        # Receive the data in small chunks and retransmit it
        while True:
            data = connection.recv(segmentSize)
            data.size()
            print(type(data))

    finally:
        # Clean up the connection
        connection.close()