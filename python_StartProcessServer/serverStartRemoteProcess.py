import socket
import sys
from subprocess import Popen

#############################
# Server IP and port Number
#############################
#ipAddress = "localhost"
ipAddress = "10.6.88.158"
portNumber = 3333

#############################
# Start Bat
#############################
startBat = "D:\\temp\\batch.bat"

#############################
# Kill Bat
#############################
KillBat = "D:\\temp\\batch1.bat"

#############################
# Server Run
#############################
# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the port
server_address = (ipAddress, portNumber)
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

        # Receive the data in small chunks and retransmit it
        while True:
            data = connection.recv(255).decode()
            print(sys.stderr, 'received "%s"' % data)
            if (data == "start")  :
                print(sys.stderr, 'receive start')
                p = Popen(startBat)
                stdout, stderr = p.communicate()
            elif (data == "stop") :
                print(sys.stderr, 'receive stop')
                p = Popen(KillBat)
                stdout, stderr = p.communicate()
            else:
                print (sys.stderr, 'no signal from', client_address)
                break

    finally:
        # Clean up the connection
        connection.close()