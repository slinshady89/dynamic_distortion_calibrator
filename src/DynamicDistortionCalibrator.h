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
		void findRawDistortion();
		void saveRawDistortion();
		void loadRawDistortion();

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

		int findPixelSize();
		cameraArea findCameraArea(int pixelSize);
		void createImages(int pixelSize);
};