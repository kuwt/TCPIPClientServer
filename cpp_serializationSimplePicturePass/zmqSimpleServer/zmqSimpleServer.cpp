// zmq 
// server using zmq for simple message passing
//
#include <zmq.hpp>
#include <string>
#include <iostream>

int main()
{
	//  Prepare our context and socket
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	socket.bind ("tcp://*:6666");
	//socket.bind("ipc:///tmp/test");

	while (true) 
	{
		zmq::message_t request;

		//  Wait for next request from client
		socket.recv(&request);
		std::cout << "Received Hello" << std::endl;

		//  Send reply back to client
		zmq::message_t reply(5);
		memcpy((void *)reply.data(), "World", 5);
		socket.send(reply);
	}
	socket.close();
	return 0;
}