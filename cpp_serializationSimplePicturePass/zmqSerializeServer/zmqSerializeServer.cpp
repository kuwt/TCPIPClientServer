// zmqSerializeServer.cpp : Defines the entry point for the console application.
//

#include <zmq.hpp>
#include <string.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "CMatSerialization.h"
#include "cereal/cereal.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/memory.hpp"
#include "fstream"
#include "iostream"

int main()
{
	//  Prepare our context and socket
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	socket.bind("tcp://*:6666");
	//socket.bind("ipc:///tmp/test");

	while (true)
	{
		std::string msgStr;
		{
			zmq::message_t message;
			socket.recv(&message);
			msgStr = std::string((char*)message.data(), message.size());
		}
		cv::Mat loaded_data;
		{
			std::stringstream iss;
			iss.str(msgStr);
			cereal::BinaryInputArchive iar(iss);
			iar(loaded_data);
		}

		{
			std::cout << "waiting for your key press on the image." << "\n";
			cv::imshow("load", loaded_data);
			cv::waitKey(0);
		}
		
		{
			zmq::message_t reply(5);
			memcpy((void *)reply.data(), "World", 5);
			socket.send(reply);
		}
	}
	socket.close();
	return 0;
}