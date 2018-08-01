#include "stdafx.h"
#include <WINSOCK2.H>
#include <iostream>
#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp>

#pragma  comment(lib,"ws2_32.lib")

int main(int argc, char* argv[])
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}


	/*************************
	//construct
	************************/
	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket !\n");
		return 0;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8888);
	serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	/*************************
	//Connect
	************************/
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !\n");
		closesocket(sclient);
		return 0;
	}

	/*************************
	//Send + recv
	************************/
	while (true)
	{
		// prepare img
		std::string imagePath;
		std::cout << "1|2|3|4|5" << "\n";
		int imageId;
		std::cin >> imageId;

		imagePath = std::string("wall") + std::to_string(imageId) + ".jpg";
		cv::Mat image_src_gray = cv::imread(imagePath, CV_LOAD_IMAGE_GRAYSCALE);

		cv::imshow("client", image_src_gray);
		cvWaitKey(1);

		//send
		const int bufferSize = 640 * 480;
		char sendData[bufferSize];
		memset(sendData, 0, bufferSize * sizeof(char));
		for (int i = 0; i < image_src_gray.size().height; i++)
		{
			for (int j = 0; j < image_src_gray.size().width; j++)
			{
				sendData[image_src_gray.size().width * i + j] = image_src_gray.at<unsigned char>(i, j);
			}
		}
		send(sclient, sendData, bufferSize, 0);

		//recv feedback
		char recData[255];
		int ret = recv(sclient, recData, 255, 0);
		if (ret > 0)
		{
			recData[ret] = 0x00;
			printf(recData);
		}
	}

	closesocket(sclient);
	WSACleanup();
	return 0;
}
