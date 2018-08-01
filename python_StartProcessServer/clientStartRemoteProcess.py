import socket
import sys

#############################
# Server IP and port Number
#############################
ipAddress = "10.6.88.158"
portNumber = 3333

#############################
# Client Run
#############################
# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = (ipAddress, portNumber)
print (sys.stderr, 'connecting to %s port %s' % server_address)
sock.connect(server_address)

try:
    # Send data
    message = 'start'
    print(sys.stderr, 'sending "%s"' % message)
    sock.sendall(message.encode())

finally:
    print(sys.stderr, 'closing socket')
    sock.close()
