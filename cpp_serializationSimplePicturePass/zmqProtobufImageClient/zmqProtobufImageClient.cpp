// zmq + protobuf
// client using zmq for protobuf message
//

#include <zmq.hpp>
#include <string.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "../zmqProtobufImageServer/objectInfos.pb.h"

int main(void)
{
	zmq::context_t context(1);

	//  Socket to talk to server
	printf("Connecting to server \n");
	zmq::socket_t sock(context, ZMQ_REQ);
	sock.connect("tcp://localhost:6666");

	// initialise a foo and set some properties
	GOOGLE_PROTOBUF_VERIFY_VERSION;

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
		//receive reply
		zmq::message_t reply;
		sock.recv(&reply, 0);
		std::string  msgStr = std::string((char*)reply.data(), reply.size());

		//unserialize
		proto::objectInfos stObjectInfos;
		stObjectInfos.ParseFromString(msgStr);
		
		for (int i = 0; i < stObjectInfos.infos_size(); ++i)
		{
			std::cout << stObjectInfos.infos(i).objclass() << " "
				<< stObjectInfos.infos(i).score() << " "
				<< stObjectInfos.infos(i).tlx() << " "
				<< stObjectInfos.infos(i).tly() << " "
				<< stObjectInfos.infos(i).brx() << " "
				<< stObjectInfos.infos(i).bry() << " "
				<< "\n";
		}
	}
	sock.close();
	system("pause");
	return 0;
}