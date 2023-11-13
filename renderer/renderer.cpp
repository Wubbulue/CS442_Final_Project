#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <bitset>

#define MAT_PATH "../example.mat"
#define OUT_VIDEO_PATH "../example_video.avi"

#define VIDEO_FPS 1.0

bool is_big_endian(void)
{
	union {
		uint32_t i;
		char c[4];
	} bint = { 0x01020304 };

	return bint.c[0] == 1;
}

int main(int argc, char** argv) {
	std::ifstream matFile(MAT_PATH, std::ios::binary);

	//Read header
	uint32_t numFrames;
	uint32_t numRows;
	uint32_t numCols;

	matFile.read(reinterpret_cast<char*>(&numFrames), sizeof(numFrames));
	matFile.read(reinterpret_cast<char*>(&numRows), sizeof(numRows));
	matFile.read(reinterpret_cast<char*>(&numCols), sizeof(numCols));

	std::vector<cv::Mat> frames;


	const bool bigEndian = is_big_endian();

	for (int frame = 0; frame < numFrames; frame++) {
		cv::Mat currentFrame(numRows,numCols,CV_8UC1);
		for (int row = 0; row < numRows; row++) {
			for (int colByte = 0; colByte < numCols/8; colByte++) {
				uint8_t byte;
				matFile.read(reinterpret_cast<char*>(&byte), 1);
				static int specialPrint = 150;
				std::bitset<8> bits = byte;

				if (!bigEndian) {
					for (int bit = 0; bit<8; bit++) {
						uint8_t val = bits[bit] ? 0 : 255;
						currentFrame.at<uint8_t>(cv::Point(colByte*8+bit, row)) = val;
					}
				}
				else {
					for (int bit = 0; bit<8; bit++) {
						uint8_t val = bits[7-bit] ? 0 : 255;
						currentFrame.at<uint8_t>(cv::Point(colByte*8+bit, row)) = val;
					}
				}

			}

		}
		frames.push_back(currentFrame);
	}

	cv::VideoWriter video(OUT_VIDEO_PATH, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),10,cv::Size(numCols,numRows));


	for (auto& frame : frames) {
		cv::Mat bgrImage;
		cv::cvtColor(frame, bgrImage, cv::COLOR_GRAY2BGR);
		video.write(frame);
	}

	video.release();

}