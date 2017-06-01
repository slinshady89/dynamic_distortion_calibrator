#include "ofMain.h"
#include "ofApp.h"
#include "PixelSizeDetector.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);// <-------- setup the GL context

	// set up pixelSize and pointer to pixelSize
	int pixelSize = 0;
	int *pixelSizePointer = &pixelSize;

	// create app for pixel size detection
	auto app = make_shared<PixelSizeDetector>();
	// set the app's pointer to the outside pixelSize variable
	app->setPixelSizePointer(pixelSizePointer);
	ofRunApp(app); // run app, closes once pixel found
	// print pixel size to screen (start programm with Strg + F5)
	std::cout << "pixelSize = " << pixelSize << "\n";	
}