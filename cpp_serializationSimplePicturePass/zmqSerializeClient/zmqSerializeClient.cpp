// zmqSerializeClient.cpp : Defines the entry point for the console application.
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
	zmq::context_t context(1);
	zmq::socket_t sock(context, ZMQ_REQ);
	sock.connect("tcp://localhost:6666");
	//sock.connect("ipc:///tmp/test");

	std::string data;
	{
		std::string imagePath;
		std::cout << "1|2|3|4|5" << "\n";
		int imageId;
		std::cin >> imageId;

		imagePath = std::string("wall") + std::to_string(imageId) + ".jpg";

		cv::Mat Imgdata = cv::imread(imagePath, CV_LOAD_IMAGE_GRAYSCALE);
		std::stringstream oss;
		cereal::BinaryOutputArchive oar(oss);
		oar(Imgdata);
		data = std::move(oss.str());
	}

	{
		zmq::message_t message(data.size());
		memcpy(message.data(), data.c_str(), data.size());
		sock.send(message);
	}

	{
		zmq::message_t reply;
		sock.recv(&reply, 0);
		printf("Received %d\n", 0);
	}
	sock.close();

	return 0;
}
