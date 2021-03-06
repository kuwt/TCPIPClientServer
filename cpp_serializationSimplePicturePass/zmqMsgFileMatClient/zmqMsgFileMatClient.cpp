// zmq + opencv
// client using zmq for simple message with file system image passing
//

#include <zmq.hpp>
#include <string.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "fstream"
#include "iostream"
int main()
{
	zmq::context_t context(1);
	zmq::socket_t sock(context, ZMQ_REQ);
	sock.connect("tcp://localhost:6666");
	cv::Mat Imgdata;
	{
		std::string imagePath;
		std::cout << "1|2|3|4|5" << "\n";
		int imageId;
		std::cin >> imageId;
		imagePath = std::string("wall") + std::to_string(imageId) + ".jpg";
		Imgdata = cv::imread(imagePath, CV_LOAD_IMAGE_GRAYSCALE);
	}

	std::string bufferPath;
	{
		bufferPath = "D:\\temp\\log\\temp.bmp";
		cv::imwrite(bufferPath, Imgdata);
	}
	// send
	{
		std::string data = bufferPath;
		zmq::message_t message(data.size());
		memcpy(message.data(), data.c_str(), data.size());
		sock.send(message);
	}

	//reply
	{
		zmq::message_t reply;
		sock.recv(&reply, 0);
		printf("Received reply\n");
	}
	sock.close();
	system("pause");
	return 0;
}
