#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <fstream>

//This is a path to an image that we will convert to a mat file
//#define IMAGE_PATH "../shrek_bigger.jpg"
#define IMAGE_PATH "../shrek.jpg"
#define OUT_PATH "../shrek.png"
#define OUT_MAT_PATH "../shrek.mat"


int main(int argc, char** argv) {

	cv::Mat shrekGray = cv::imread(IMAGE_PATH, cv::IMREAD_GRAYSCALE);


	//Convert shrek to a series of binary images with different thresholds
	std::vector<int> thresholds = { 20,30,50,80,100,120,150,170,190 };
	std::vector<cv::Mat> shrekBinaries(thresholds.size());
	for (int i = 0; i < thresholds.size(); i++) {
		cv::threshold(shrekGray, shrekBinaries[i], thresholds[i], 255, cv::THRESH_BINARY);
	}


	std::ofstream outMat(OUT_MAT_PATH);

	const uint32_t numFrames = thresholds.size();
	const uint32_t numRows = shrekGray.rows;


	const uint32_t numCols = shrekGray.cols;

	//write a multiple of 8 for cols
	int remainder = numCols % 8;
	uint32_t numColsActual = numCols + 8 - remainder;
	if (remainder == 0) {
		numColsActual = numCols;
	}

	//Write header (num frames, num rows, num cols)
	outMat.write(reinterpret_cast<const char*>(&numFrames), sizeof(numFrames));
	outMat.write(reinterpret_cast<const char*>(&numRows), sizeof(numRows));
	outMat.write(reinterpret_cast<const char*>(&numColsActual), sizeof(numColsActual));

	for (int frame = 0; frame < numFrames; frame++) {
		auto& currentFrame = shrekBinaries[frame];
		int bytesPerFrame = 0;
		for (int row = 0; row < numRows; row++) {
			std::vector<uint8_t> binaryRow (numColsActual,0);
			for (int col = 0; col < numCols; col++) {
				binaryRow[col] = (currentFrame.at<uint8_t>(cv::Point(col, row)));
			}

			std::vector<uint8_t> rowBytes(binaryRow.size() / 8);

			for (int numByte = 0; numByte < rowBytes.size(); numByte++) {
				uint8_t byte = 0;

				for (int bit = 0; bit < 8; ++bit)
					if (!binaryRow[bit+numByte*8])
						byte |= 1 << bit;

				rowBytes[numByte] = byte;
			}
			outMat.write(reinterpret_cast<const char*>(rowBytes.data()), rowBytes.size());
			bytesPerFrame += rowBytes.size();
		}
		printf("Bytes per frame: %d\n", bytesPerFrame);
	}

	outMat.close();


	return 0;



}
