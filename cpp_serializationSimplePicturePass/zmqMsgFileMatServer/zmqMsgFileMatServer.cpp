#include <zmq.hpp>
#include <string.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "fstream"
#include "iostream"
int main()
{
	//  Prepare our context and socket
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	socket.bind("tcp://*:6666");
	while (true)
	{
		//  receive message
		zmq::message_t message;
		if (socket.recv(&message))
		{
			std::string  msgStr = std::string((char*)message.data(), message.size());
			//   restore cv::mat from harddisk
			cv::Mat loaded_data = cv::imread(msgStr, CV_LOAD_IMAGE_GRAYSCALE);
			//  show cv::mat
			{
				std::cout << "waiting for your key press on the image." << "\n";
				cv::imshow("load", loaded_data);
				cv::waitKey(0);
			}
		}		
		//send reply
		{
			std::string data = "reply";
			zmq::message_t message(data.size());
			memcpy(message.data(), data.c_str(), data.size());
			socket.send(message);
		}
	}
	socket.close();
	return 0;
}