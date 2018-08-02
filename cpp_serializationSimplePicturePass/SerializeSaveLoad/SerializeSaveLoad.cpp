// SerializeSimplePicturePass.cpp : Defines the entry point for the console application.
//

#include <opencv2/opencv.hpp>
#include "CMatSerialization.h"
#include "cereal/cereal.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/memory.hpp"
#include "fstream"
#include "iostream"
int main()
{
	std::cout << "s/l?" << "\n";
	std::string mode;
	std::cin >> mode;

	if (mode == "s")
	{
		cv::Mat data = cv::imread("wall1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		{
			std::ofstream file("file.bin", std::ios::binary);
			cereal::BinaryOutputArchive ar(file);
			ar(data);
		}

	}
	else if (mode == "l")
	{
		// Load the data from the disk again:
		cv::Mat loaded_data;
		{
			std::ifstream file("file.bin", std::ios::binary);
			cereal::BinaryInputArchive ar(file);
			ar(loaded_data);
		}
		cv::imshow("load", loaded_data);
		cv::waitKey(0);
	}
	return 0;
}

