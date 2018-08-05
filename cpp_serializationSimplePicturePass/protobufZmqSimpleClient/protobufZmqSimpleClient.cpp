
#include <zmq.hpp>
#include <string.h>
#include <stdio.h>

#include "../protobufZmqSimpleServer/foo.pb.h"

int main(void)
{
	zmq::context_t context(1);

	//  Socket to talk to server
	printf("Connecting to server \n");
	zmq::socket_t sock(context, ZMQ_REQ);
	sock.connect("tcp://localhost:6666");

	// initialise a foo and set some properties
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	prototest::Foo foo;
	foo.set_id(4);
	foo.set_bar("narf");
	std::string buf;
	foo.SerializeToString(&buf);

	// send
	{
		zmq::message_t message(buf.size());
		memcpy(message.data(), buf.data(), buf.size());
		sock.send(message);
	}

	zmq::message_t reply;
	sock.recv(&reply, 0);
	
	sock.close();
	return 0;
}