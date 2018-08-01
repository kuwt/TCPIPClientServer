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
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//开始监听
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);

	printf("Wait for Connection...\n");
	do
	{
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
	} while (sClient == INVALID_SOCKET);
	printf("Receive a connection：%s \r\n", inet_ntoa(remoteAddr.sin_addr));

	const int bufferSize = 640 * 480;
	
	cv::Mat image_src = cv::Mat(cv::Size(640, 480), CV_8U);

	int SegmentStartIdx = 0;
	while (true)
	{
		const int segmentSize = 1024;
		const int bufferSize = 640 * 480;
		char revData[segmentSize];
		memset(revData, 0, segmentSize * sizeof(char));
		int recvSize = recv(sClient, revData, segmentSize, 0);
		if (recvSize > 0)
		{
			int bufferIdx = SegmentStartIdx;
			for (int i = 0; i < recvSize; ++i)
			{
				char datum = revData[i];
				int imageIdY = bufferIdx / image_src.size().width;
				int imageIdX = bufferIdx - imageIdY * image_src.size().width;
				image_src.at<unsigned char>(imageIdY, imageIdX) = datum;
				bufferIdx++;
			}
			SegmentStartIdx = recvSize + SegmentStartIdx;
		}
		if (SegmentStartIdx >= bufferSize)
		{
			cv::imshow("server", image_src);
			cvWaitKey(1);
			SegmentStartIdx = 0;
		}
	}
	closesocket(slisten);
	WSACleanup();
	return 0;
}