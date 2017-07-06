/*#include "ofMain.h"
#include "PixelSizeDetector.h"
#include "CameraAreaDetector.h"
#include "cameraArea.h"*/
#pragma warning(disable: 4996)

#include "DynamicDistortionCalibrator.h"

extern int WHITEVALUE = 200;

//========================================================================
int main( ){
	//_________________ Detect minimal pixel size _____________________________
	/*ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);// <-------- setup the GL context

	// set up pixelSize and pointer to pixelSize
	int pixelSize = 0;
	int *pixelSizePointer = &pixelSize;

	// create app for pixel size detection
	auto pixelSizeDetector = make_shared<PixelSizeDetector>();
	// set the app's pointer to the outside pixelSize variable
	pixelSizeDetector->setPixelSizePointer(pixelSizePointer);
	ofRunApp(pixelSizeDetector); // run app, closes once pixel found
	// print pixel size to screen (start programm with Strg + F5)
	std::cout << "found pixelSize = " << pixelSize << "\n";
	*/
	/*int pixelSize = 10; //TODO delete, debug only
	//_________________ Detect camera area ____________________________________
	ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);// <-------- setup the GL context
	cameraArea area;
	cameraArea *areaPointer = &area;
	auto cameraAreaDetector = make_shared<CameraAreaDetector>();
	// set the app's pointer
	cameraAreaDetector->setCameraAreaPointerAndPixelSize(areaPointer, pixelSize);
	ofRunApp(cameraAreaDetector);
	std::cout << "found area \n";
	*/

	int** matchX;
	int** matchY;

	DynamicDistortionCalibrator dynDistCal(1920, 1080);


	dynDistCal.setSpacing(89);
	dynDistCal.setResolutionHeight(720);
	dynDistCal.setResolutionWidth(1280);
	dynDistCal.setCannyLowerThreshold(40);
	dynDistCal.setCannyUpperThreshold(110);
	//dynDistCal.findRawDistortion(matchX, matchY);
	//dynDistCal.saveRawDistortion("data/maps.txt");
	dynDistCal.loadRawDistortion("data/maps.txt");
	dynDistCal.saveRawDistortion("data/maps2.txt");

	ofxCvGrayscaleImage distortedImage = dynDistCal.createImage(true, true);
	std::cout << "created  and saved image\n";
	

	ofImage undistortedImage = dynDistCal.undistort(distortedImage.getCvImage(), matchX, matchY);

	// save image as cv::imshow doesn't work currently
	undistortedImage.save("undistortedTestImage.jpg", OF_IMAGE_QUALITY_BEST);
}