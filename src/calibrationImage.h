#pragma once
#include "Constants.h"

struct calibrationImage {
	// contains the ground truth image, aka the screen content
	cv::Mat _groundTruth;

	// contains the image seen by the camera
	cv::Mat _image;

	// distortion in x direction
	cv::Mat _distortionX;
	// distortion in y direction
	cv::Mat _distortionY;

	// match x-indices
	cv::Mat _matchX;
	cv::Mat _matchY;

	// initial position, aka point where lines cross, estimated middle
	pos _initialPosition;

	// sizes of groundTruth and image
	int _gtWidth;
	int _gtHeight;
	int _imWidth;
	int _imHeight;

	// offset of initial line from left image border
	int* _verticalOffsets;
	// offset of initial line from upper image border
	int* _horizontalOffsets;
};