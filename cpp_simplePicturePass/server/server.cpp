#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include <opencv2/opencv.hpp>

#pragma comment(lib,"ws2_32.lib")

int main(int argc, char* argv[])
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	/*************************
	//construct
	************************/
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	/*************************
	//bind
	************************/
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	/*************************
	//listen
	************************/
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	/*************************
	//Connect
	************************/
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);

	printf("Wait for Connection...\n");
	do
	{
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
	} while (sClient == INVALID_SOCKET);
	printf("Receive a connection：%s \r\n", inet_ntoa(remoteAddr.sin_addr));

	
	/*************************
	//Receive
	************************/
	cv::Mat img = cv::Mat(cv::Size(640, 480), CV_8U);
	int  imgSize = img.total()*img.elemSize();
	std::vector<char> sockData(imgSize, 0);
	//char *sockData = new char[imgSize];
	while (true)
	{
		// Receive datastream
		int bytes = 0;
		for (int i = 0; i < imgSize; i += bytes)
		{
			if ((bytes = recv(sClient, &sockData[0] + i, imgSize - i, 0)) == -1)
			{
				break;
			}
		}

		// Assign pixel value to img
		int ptr = 0;
		for (int i = 0; i < img.rows; i++)
		{
			for (int j = 0; j < img.cols; j++) 
			{
				img.at<unsigned char>(i, j) = sockData[ptr];
				ptr = ptr + 1;
			}
		}

		cv::imshow("server", img);
		cvWaitKey(1);

		//feedback
		char * sendData = "Server: Image Well Received\n";
		send(sClient, sendData, strlen(sendData), 0);
	}

	/*************************
	//close
	************************/
	closesocket(slisten);
	WSACleanup();
	return 0;
}