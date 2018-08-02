#pragma once

#ifndef CMATSERIALIZATION_H
#define CMATSERIALIZATION_H

#include <opencv2/core/core.hpp>

//more details see
//1. http://www.patrikhuber.ch/blog/6-serialising-opencv-matrices-using-boost-and-cereal
//2. http://uscilab.github.io/cereal/serialization_functions.html

namespace cereal {
	//External serialization functions should be placed either
	//in the same namespace as the types they serialize or
	//in the cereal namespace so that the compiler can find them properl

	template<class Archive>
	void save(Archive& ar, const cv::Mat& mat)
	{
		int rows, cols, type;
		bool continuous;

		rows = mat.rows;
		cols = mat.cols;
		type = mat.type();
		continuous = mat.isContinuous();

		ar & rows & cols & type & continuous;

		if (continuous) {
			const int data_size = rows*cols *static_cast<int>(mat.elemSize());
			auto mat_data = cereal::binary_data(mat.ptr(), data_size);
			ar(mat_data);
		}
		else {
			const int row_size = cols * static_cast<int>(mat.elemSize());
			for (int i = 0; i < rows; i++) {
				auto row_data = cereal::binary_data(mat.ptr(i), row_size);
				ar(row_data);
			}
		}
	}

	template<class Archive>
	void load(Archive& ar, cv::Mat& mat)
	{
		int rows, cols, type;
		bool continuous;

		ar & rows & cols & type & continuous;

		if (continuous) {
			mat.create(rows, cols, type);
			const int data_size = rows * cols * static_cast<int>(mat.elemSize());
			auto mat_data = cereal::binary_data(mat.ptr(), data_size);
			ar & mat_data;
		}
		else {
			mat.create(rows, cols, type);
			const int row_size = cols * static_cast<int>(mat.elemSize());
			for (int i = 0; i < rows; i++) {
				auto row_data = cereal::binary_data(mat.ptr(i), row_size);
				ar & row_data;
			}
		}
	}

}//end of namespace cereal


#endif // CMATSERIALIZATION_H
