#pragma once
#include "cameraArea.h"
#include "PixelSizeDetector.h"
#include "CameraAreaDetector.h"
#include "ImageCreator.h"
#include "Constants.h"


class DynamicDistortionCalibrator {
	public:
		// constructor
		DynamicDistortionCalibrator(int windowWidth , int windowHeight);
		// destructor
		~DynamicDistortionCalibrator();

		// stuff for raw distortion aka local offset in x & y direction
		void findRawDistortion(int** &matchX, int** &matchY);
		void saveRawDistortion();
		void loadRawDistortion();
		// undistorts the given image, using the given distortion maps
		cv::Mat undistort(cv::Mat distortedImage, int** matchX, int** matchY);
		// creates an image for distortion testing purposes
		cv::Mat createImage(bool vert = true, bool hor = true);

		// setter for the spacing in the cameraAreaDetector
		void setSpacing(int spacing);
		// getter for the spacing in the cameraAreaDetector
		int getSpacing();

		// setter for camera resolution's width
		void setResolutionWidth(int resolutionWidth);
		// getter for camera resolution's width
		int getResolutionWidth();

		// setter for camera resolution's height
		void setResolutionHeight(int resolutionHeight);
		// getter for camera resolution's height
		int getResolutionHeight();

		// setter for lower threshold for canny edge detection
		void setCannyLowerThreshold(int lowerThreshold);
		// getter for lower threshold for canny edge detection
		int getCannyLowerThreshold();

		// setter for upper threshold for canny edge detection
		void setCannyUpperThreshold(int upperThreshold);
		// getter for upper threshold for canny edge detection
		int getCannyUpperThreshold();

	private:
		cameraArea _area;
		int _windowHeight, _windowWidth;
		calibrationImage _vertical, _horizontal;
		int _spacing;
		int _resolutionWidth, _resolutionHeight;
		// thresholds for canny detector
		int _cannyLower, _cannyUpper;
		int _pixelSize;

		int findPixelSize();
		cameraArea findCameraArea();

		// returns the undistorted image with interpolated pixels in the inner of the frame
		cv::Mat interpolateImage(cv::Mat undistedImage);
		// returns the undistorted image
		cv::Mat mappingImage(cv::Mat distordedImage, int** matchY, int** matchX);
};