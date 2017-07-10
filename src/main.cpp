/*#include "ofMain.h"
#include "PixelSizeDetector.h"
#include "CameraAreaDetector.h"
#include "cameraArea.h"*/
#pragma once
#pragma warning(disable: 4996)

#include "DynamicDistortionCalibrator.h"
#include <opencv2/highgui/highgui.hpp>

extern int WHITEVALUE = 200;

//========================================================================
int main( ){
	int** matchX;
	int** matchY;

	DynamicDistortionCalibrator dynDistCal(1920, 1080);

	dynDistCal.setSpacing(101);
	dynDistCal.setResolutionHeight(720);
	dynDistCal.setResolutionWidth(1280);
	dynDistCal.setCannyLowerThreshold(40);
	dynDistCal.setCannyUpperThreshold(110);
	dynDistCal.setJump(3);
	//dynDistCal.findRawDistortion(matchX, matchY);
	//dynDistCal.saveRawDistortion("data/maps.txt");
	dynDistCal.loadRawDistortion("data/maps.txt");

	matchX = dynDistCal.getMapX();
	matchY = dynDistCal.getMapY();
	std::cout << "loaded maps\n";

	dynDistCal.setMaps(matchX, matchY);
	dynDistCal.saveRawDistortion("data/mapsInterpolated.txt");

	ofxCvGrayscaleImage distortedImage;
	/*distortedImage.allocate(1280, 720);
	distortedImage = dynDistCal.createImage(true, true);
	*/
	//ofImage img;
	/*img.setFromPixels(distortedImage.getPixels());
	img.save("distortedImage.jpg");*/
	/*
	img.load("distortedImage.jpg");

	distortedImage.setFromPixels(img.getPixels());

	cv::Mat distorted;
	distorted = distortedImage.getCvImage();

	std::cout << "created and saved image\n";
	*/
	//dynDistCal.saveImageAsTxt(distortedImage, "data/distortedImageTest.jpg");
	//std::cout << "saved image\n";
	/*ofPixels pix = dynDistCal.loadImageAsTxt("data/distortedImage.jpg");
	std::cout << "loaded image\n";
	dynDistCal.saveImageAsTxt(img, "data/distortedImage2.jpg");
	std::cout << "saved image\n";*/

	

	//ofImage undistortedImage = dynDistCal.undistort(distorted, matchX, matchY);

	// save image as cv::imshow doesn't work currently
	//undistortedImage.save("undistortedTestImage.jpg", OF_IMAGE_QUALITY_BEST);
	
}