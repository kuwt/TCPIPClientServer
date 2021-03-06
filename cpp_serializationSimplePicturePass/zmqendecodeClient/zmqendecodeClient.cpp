// zmq + opencv
// client using zmq for image message with encode decode mechanism
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

	std::vector<uchar> data_encode;
	{
		std::string imagePath;
		std::cout << "1|2|3|4|5" << "\n";
		int imageId;
		std::cin >> imageId;
		imagePath = std::string("wall") + std::to_string(imageId) + ".jpg";
		cv::Mat Imgdata = cv::imread(imagePath, CV_LOAD_IMAGE_GRAYSCALE);

		cv::imencode(".bmp", Imgdata, data_encode);
	}
	// send
	{
		zmq::message_t message(data_encode.size());
		memcpy(message.data(), data_encode.data(), data_encode.size());
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
