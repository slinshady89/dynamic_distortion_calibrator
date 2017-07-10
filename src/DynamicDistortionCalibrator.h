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
		void saveRawDistortion(string path);
		void loadRawDistortion(string path);
		// undistorts the given image, using the given distortion maps
		ofImage undistort(cv::Mat distortedImage, int** matchX, int** matchY);
		// creates an image for distortion testing purposes
		ofxCvGrayscaleImage createImage(bool vert = true, bool hor = true);

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

		// setter for jump width
		void setJump(int jump);
		// getter for jump width
		int getJump();

		// workaround since the openframeworks internal img.save, img.saveImage, and ofSaveImage(img)
		// don't work
		void saveImageAsTxt(ofxCvGrayscaleImage pix, string fileName);
		ofPixels loadImageAsTxt(string fileName);

		// getter for maps, no setter as it should be either found or loaded
		int** getMapX();
		int** getMapY();

		// FOR DEBUG PURPOSES ONLY
		void setMaps(int** mapX, int** mapY);

	private:
		cameraArea _area;
		int _windowHeight, _windowWidth;
		calibrationImage _vertical, _horizontal;
		int _spacing;
		int _resolutionWidth, _resolutionHeight;
		// thresholds for canny detector
		int _cannyLower, _cannyUpper;
		int _pixelSize;
		int _jump;

		int findPixelSize();
		cameraArea findCameraArea();

		// returns the undistorted image with interpolated pixels in the inner of the frame
		cv::Mat interpolateImage(cv::Mat undistedImage);
		// returns the undistorted image
		cv::Mat mappingImage(cv::Mat distordedImage, int** matchY, int** matchX);
		// breaks up a line into an array
		int* stringToIntArray(string line);
};