// zmqSerializeServer.cpp : Defines the entry point for the console application.
//

#include <zmq.hpp>
#include <string.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "..\zmqSerializeClient\CMatSerialization.h"
#include "..\zmqSerializeClient\CObjLocSerialization.h"
#include "..\zmqSerializeClient\ObjLoc.h"
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
		//  receive message
		std::string msgStr;
		{
			zmq::message_t message;
			socket.recv(&message);
			msgStr = std::string((char*)message.data(), message.size());
		}

		//  unserialize to cv::mat
		cv::Mat loaded_data;
		{
			std::stringstream iss;
			iss.str(msgStr);
			cereal::BinaryInputArchive iar(iss);
			iar(loaded_data);
		}

		//  show cv::mat
		{
			std::cout << "waiting for your key press on the image." << "\n";
			cv::imshow("load", loaded_data);
			cv::waitKey(0);
		}
		
		// reply objLocs;
		{
			// prepare objLocs;
			std::vector<ObjLoc> vObjLocs;
			{
				ObjLoc Loc;
				Loc.objClass = 0; Loc.score = 0.6; Loc.tlx = 0; Loc.tly = 2; Loc.brx = 123; Loc.bry = 102;
				vObjLocs.push_back(Loc);
			}
			{
				ObjLoc Loc;
				Loc.objClass = 0; Loc.score = 0.6; Loc.tlx = 120; Loc.tly = 232; Loc.brx = 1233; Loc.bry = 1012;
				vObjLocs.push_back(Loc);
			}

			//  serialize to string
			std::string data;
			{
				std::stringstream oss;
				cereal::BinaryOutputArchive oar(oss);
				int size = vObjLocs.size();
				oar(size);
				for (int j = 0; j < vObjLocs.size(); j++) 
				{
					oar(vObjLocs.at(j));
				}
				data = std::move(oss.str());
			}
		
			//  send message
			zmq::message_t message(data.size());
			memcpy(message.data(), data.c_str(), data.size());
			socket.send(message);
		}
	}
	socket.close();
	return 0;
}