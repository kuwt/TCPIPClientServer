// zmq + protobuf
// server using zmq for protobuf message
//

#include <zmq.hpp>
#include <string>
#include <iostream>
#include "foo.pb.h"
int main()
{
	//  Prepare our context and socket
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	socket.bind("tcp://*:6666");
	//socket.bind("ipc:///tmp/test");
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	while (true)
	{
		zmq::message_t request;
		socket.recv(&request);
		std::string  msgStr = std::string((char*)request.data(), request.size());

		prototest::Foo foo;
		foo.ParseFromString(msgStr);
		std::cout << foo.id() << " " << foo.bar() << "\n";

		zmq::message_t reply(5);
		memcpy((void *)reply.data(), "reply", 5);
		socket.send(reply);
	}
	socket.close();
	return 0;
}