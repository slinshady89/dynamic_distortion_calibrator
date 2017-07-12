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
	cv::Mat distorted;
	ofxCvGrayscaleImage distortedImage;
	ofImage img;

	DynamicDistortionCalibrator dynDistCal(1920, 1080);

	dynDistCal.setSpacing(70);
	dynDistCal.setResolutionHeight(720);
	dynDistCal.setResolutionWidth(1280);
	dynDistCal.setCannyLowerThreshold(40);
	dynDistCal.setCannyUpperThreshold(110);
	dynDistCal.setJump(1);

	// PART 1, FIRST MAIN CALL ONLY RUN FROM HERE TO PART 2!
	// PUT THIS IN COMMENT WHEN RUNNING PART 2 OR 3!
	// OPENFRAMEWORKS FAILURE!
	/*
	dynDistCal.findRawDistortion(matchX, matchY);
	dynDistCal.saveRawDistortion("data/mapsJump1.txt");
	*/

	
	// PART 2, SECOND MAIN CALL
	// RUN FROM HERE TO PART 3
	/*
	distortedImage.allocate(1280, 720);
	distortedImage = dynDistCal.createImage(true, true);
	img.setFromPixels(distortedImage.getPixels());
	img.save("distortedImage.jpg");
	*/

	// PART 3, THIRD AND LAST MAIN CALL RUN FROM HERE TO THE END
	/*
	dynDistCal.loadRawDistortion("data/mapsJump1.txt");

	matchX = dynDistCal.getMapX();
	matchY = dynDistCal.getMapY();
	std::cout << "loaded maps\n";

	matchX = dynDistCal.interpolateLines(matchX, true);
	matchY = dynDistCal.interpolateLines(matchY, false);

	dynDistCal.setMaps(matchX, matchY);
	dynDistCal.saveRawDistortion("data/mapsJump1Interpolated.txt");
	
	img.load("distortedImage.jpg");
	distortedImage.setFromPixels(img.getPixels());
	distorted = distortedImage.getCvImage();

	ofImage undistortedImage = dynDistCal.undistort(distorted, matchX, matchY);

	// save image
	undistortedImage.save("interpolatedJump1.jpg", OF_IMAGE_QUALITY_BEST);
	*/
	// END

	// If GROUND TRUTH is needed run this
	// load maps
	dynDistCal.loadRawDistortion("data/mapsJump3.txt");
	matchX = dynDistCal.getMapX();
	matchY = dynDistCal.getMapY();
	// and interpolate them
	matchX = dynDistCal.interpolateLines(matchX, true);
	matchY = dynDistCal.interpolateLines(matchY, false);
	std::cout << "loaded maps\n";

	// load screen content, has to be saved prior to this
	ofImage screenContent;
	screenContent.loadImage("screenContent.jpg");

	// run calculation on screenContent with maps
	ofImage gt;
	gt = dynDistCal.createGroundTruthFromImageAndMap(screenContent, matchX, matchY);
	gt.save("groundTruth3.jpg", ofImageQualityType::OF_IMAGE_QUALITY_BEST);
}