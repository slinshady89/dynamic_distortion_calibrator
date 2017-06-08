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

	DynamicDistortionCalibrator dynDistCal(1080, 1920);
	dynDistCal.findRawDistortion();
}