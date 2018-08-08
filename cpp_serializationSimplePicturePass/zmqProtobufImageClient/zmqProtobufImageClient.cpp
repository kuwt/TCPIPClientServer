// zmq + protobuf
// client using zmq for protobuf message
//

#include <zmq.hpp>
#include <string.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "../zmqProtobufImageServer/objectInfos.pb.h"
#include <chrono>  // for high_resolution_clock

int showImg(std::string windowName, cv::Mat img, int maxHeightShown, int maxWidthShown)
{
	if (img.empty())
	{
		std::cout << "no image" << "\n";
		return -1;
	}

	cv::Mat showImg = img;
	if (showImg.size().height > maxHeightShown)
	{
		float newHeight = maxHeightShown;
		float fy = newHeight / (float)showImg.size().height;
		float fx = fy;
		cv::resize(showImg, showImg, cv::Size(), fx, fy, cv::INTER_CUBIC);
	}
	if (showImg.size().width > maxWidthShown)
	{
		float newwidth = maxWidthShown;
		float fx = newwidth / (float)showImg.size().height;
		float fy = fx;
		cv::resize(showImg, showImg, cv::Size(), fx, fy, cv::INTER_CUBIC);
	}

	cv::imshow(windowName, showImg);
	cv::waitKey(1);
}

int main(void)
{
	zmq::context_t context(1);

	/***********************
	//  Socket to talk to server
	**********************/
	printf("Connecting to server \n");
	zmq::socket_t sock(context, ZMQ_REQ);
	sock.connect("tcp://localhost:5555");
	
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	
	auto startWholeProcess = std::chrono::high_resolution_clock::now();
	/***********************
	//  Serialize
	**********************/
	auto startEncode = std::chrono::high_resolution_clock::now();
	std::vector<uchar> data_encode;
	cv::Mat Imgdata;
	{
		std::string imagePath;
		std::cout << "1|2|3|4|5" << "\n";
		int imageId;
		std::cin >> imageId;
		imagePath = std::string("0") + std::to_string(imageId) + ".bmp";
		Imgdata = cv::imread(imagePath, CV_LOAD_IMAGE_COLOR);

		cv::imencode(".bmp", Imgdata, data_encode);
	}
	auto finishEncode = std::chrono::high_resolution_clock::now();
	
	/***********************
	// send
	**********************/
	auto startSend = std::chrono::high_resolution_clock::now();
	{
		zmq::message_t message(data_encode.size());
		memcpy(message.data(), data_encode.data(), data_encode.size());
		sock.send(message);
	}
	auto finishSend = std::chrono::high_resolution_clock::now();

	/***********************
	// get reply
	**********************/
	zmq::message_t reply;
	sock.recv(&reply, 0);
	std::string  msgStr = std::string((char*)reply.data(), reply.size());
	auto endWholeProcess = std::chrono::high_resolution_clock::now();
	//unserialize
	proto::objectInfos stObjectInfos;
	stObjectInfos.ParseFromString(msgStr);
	{
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
		cv::Mat shownImage;
		Imgdata.copyTo(shownImage);

		for (int i = 0; i < stObjectInfos.infos_size(); ++i)
		{
			cv::Rect r = cv::Rect(
				stObjectInfos.infos(i).tlx(),
				stObjectInfos.infos(i).tly(),
				stObjectInfos.infos(i).brx() - stObjectInfos.infos(i).tlx(),
				stObjectInfos.infos(i).bry() - stObjectInfos.infos(i).tly());
			cv::rectangle(shownImage, r, cv::Scalar(255, 0, 0), 5, 8, 0);
		}

		showImg("result", shownImage, 800, 800);
	}
	std::chrono::duration<double> elapsedSerialize = finishEncode - startEncode;
	std::cout << "serialize process time elapsed = " << elapsedSerialize.count() << "\n";
	std::chrono::duration<double> elapsedSend = finishSend - startSend;
	std::cout << "send time elapsed = " << elapsedSend.count() << "\n";
	std::chrono::duration<double> elapsedWholeProcess = endWholeProcess - startWholeProcess;
	std::cout << "whole process time elapsed = " << elapsedWholeProcess.count() << "\n";
	
	sock.close();
	system("pause");
	return 0;
}