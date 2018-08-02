// zmqSerializeClient.cpp : Defines the entry point for the console application.
//

#include <zmq.hpp>
#include <string.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "CMatSerialization.h"
#include "CObjLocSerialization.h"
#include "ObjLoc.h"
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

	//  serialize to data str
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

	// send
	{
		zmq::message_t message(data.size());
		memcpy(message.data(), data.c_str(), data.size());
		sock.send(message);
	}

	// receive reply
	{
		// receive message
		std::string msgStr;
		{
			zmq::message_t message;
			sock.recv(&message, 0);
			msgStr = std::string((char*)message.data(), message.size());
		}

		// unserialize to objlocs
		std::vector<ObjLoc> vObjLocs;
		{
			std::stringstream iss;
			iss.str(msgStr);
			cereal::BinaryInputArchive iar(iss);
			int size = 0;
			iar(size);
			for (int i = 0; i < size; ++i)
			{
				ObjLoc stLoc;
				iar(stLoc);
				vObjLocs.push_back(stLoc);
			}
		}

		// print objlocs
		std::cout << "print objlocs \n"; 
		for (int i = 0; i < vObjLocs.size(); ++i)
		{
			std::cout 
				<< vObjLocs.at(i).objClass << " "
				<< vObjLocs.at(i).score << " "
				<< vObjLocs.at(i).tlx << " "
				<< vObjLocs.at(i).tly << " "
				<< vObjLocs.at(i).brx << " "
				<< vObjLocs.at(i).bry << "\n";
		}
	}
	
	sock.close();
	system("pause");
	return 0;
}
