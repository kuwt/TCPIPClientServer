// zmq 
// client using zmq for simple message passing
//

#include <zmq.hpp>
#include <string.h>
#include <stdio.h>

int main(void)
{
	zmq::context_t context(1);

	//  Socket to talk to server
	printf("Connecting to hello world server…\n");
	zmq::socket_t sock(context, ZMQ_REQ);
	sock.connect("tcp://localhost:6666");
	//sock.connect("ipc:///tmp/test");

	int request_nbr;
	for (request_nbr = 0; request_nbr != 10; request_nbr++) 
	{
		zmq::message_t request((void*)"Hello", 5, NULL);
		//        zmq::msg_init_size (&request, 5);
		//        memcpy (zmq::msg_data (&request), "Hello", 5);
		printf("Sending Hello %d…\n", request_nbr);
		sock.send(&request, 0);
		//zmq::msg_close (&request);

		zmq::message_t reply;
		sock.recv(&reply, 0);
		printf("Received World %d\n", request_nbr);
	}
	sock.close();
	return 0;
}