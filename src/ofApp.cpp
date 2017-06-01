#include "ofApp.h"
#include "opencv2/objdetect/objdetect.hpp"

using namespace cv;

//--------------------------------------------------------------
void ofApp::setup() {
	// check for external webcam and use that if possible
	if (cam.listDevices().size() > 1)    cam.setDeviceID(1);
	else    cam.setDeviceID(0); 
	// frame in which the camera signal will be shown
	cam.setup(ofGetWidth(), ofGetHeight());
	ofBackground(ofColor::black);
	framerate = 60;
	/*recog = false;
	
	colorImage.allocate(320, 240);
	grayImage.allocate(320, 240);
	grayBg.allocate(320, 240);
	grayDiff.allocate(320, 240);

*/

	//set framerate (speed) of the travelling pixel
	ofSetFrameRate(framerate);
	windowHeight = ofGetWindowHeight();
	windowWidth = ofGetWindowWidth();
	pixelSize = 1;
	spiralSize = pixelSize;
	spiralDirection = 0;
	startX = ceil(windowWidth / 2.0)- ceil(pixelSize / 2.0);
	startY = ceil(windowHeight / 2.0)- ceil(pixelSize / 2.0);
	x = startX;
	y = startY;
	cout << "done with setup.\n";

	maxSize = max(ofGetWindowHeight(), ofGetWindowWidth());

}

//--------------------------------------------------------------
void ofApp::update() {
	// static update of the received Signal
	cam.update();


	calculateNextSpiralPosition();



}

//--------------------------------------------------------------
// Make a white plane on which one or more Pixel(s) are travelling 
void ofApp::draw() {


	// draw the travelling pixel
	ofSetColor(ofColor::white);
	ofDrawRectangle(x, y, pixelSize, pixelSize);
	ofSetColor(ofColor::black);


	img = cam.getPixels();


	int w = img.getWidth();
	int h = img.getHeight();

	float maxBrightness = 0; // these are used in the search
	int maxBrightnessX = 0; // for the brightest location
	int maxBrightnessY = 0;

	// Search through every pixel. If it is brighter than any
	// we've seen before, store its brightness and coordinates.
	// After testing every pixel, we'll know which is brightest!
	for (int y = 0; y<h; y++) {
		for (int x = 0; x<w; x++) {
			ofColor colorAtXY = img.getColor(x, y);
			float brightnessOfColorAtXY = colorAtXY.getBrightness();
			if (brightnessOfColorAtXY > maxBrightness) {
				maxBrightness = brightnessOfColorAtXY;
				maxBrightnessX = x;
				maxBrightnessY = y;
			}
			//file << brightnessOfColorAtXY << " ; ";
		}
		//file << endl;
	}
  //file << endl << endl << "newLine" << endl << endl;

	// Draw the image.
	ofSetColor(255);
	//img.draw(0, 0);

	// Draw a circle at the brightest location.
	ofNoFill();

	ofSetColor(ofColor::red);
	ofDrawEllipse(maxBrightnessX, maxBrightnessY, 40, 40);



	ofSetColor(ofColor::black);

	
}



//--------------------------------------------------------------
void ofApp::detectPixelSize(ofxCvColorImage &image) {


  // to find the minimum size of a pixel


	/*int width = image.getWidth();
	int height = image.getHeight();

	for (int i = 0; i < width; ) {
		for (int j = 0; j < height; ) {
			ofRect(i, j, pixelSize, pixelSize);
			j += height / 10;
		}
		i += width / 10;
	}*/


	pix = image.getPixels();
	// go over all pixels
	for (auto & pixel : pix.getPixelsIter()) {
		// and detect their colour
		pixel.getColor();
		// for debug purposes: if the colour is white something was detected
		// later: if a black pixel was recognized on a white screen
		if (!(pixel.getColor() == ofColor::black)) {
			recog = true;
			framerate = 30;
		}
		else {
			// else increase the pixel size by one
			pixelSize++;
			detectPixelSize(image);
		}
	}

	// check for pixelSize exceeding the window size
	if (pixelSize >=/* maxSize/*/10) {
		std::cout << "Error! pixelSize exceeds the size of the window!\n";
		pixelSize = ceil(/*maxSize /*/ 10.0) ;
		std::cout << "Setting pixelSize to the maxSize: " << pixelSize;
	}
	std::cout << "Found pixelSize = " << pixelSize << ".\n";
}

//--------------------------------------------------------------
void ofApp::calculateNextSpiralPosition() {
	switch (spiralDirection) {
		case 0: std::cout << "in case 0\n";
			if (y < 0) {                        // reaching upper windowborder
				x = startX - spiralSize;          // change x to left spiral side
				spiralDirection = 2;              // -> 180° turn of direction
			}
			y--;
			if (y <= startY - spiralSize) {     // dy > spiralsize will lead to 
				spiralDirection = 1;              // change of direction to left
			};
			break;
		case 1: std::cout << "in case 1\n";
			if (x < 0) {                        // reaching left windowborder
				y = startY + spiralSize;          // change x to upper spiral side
				spiralDirection = 3;              // -> 180° turn of direction
			}
			x--;
			if (x <= startX - spiralSize) {     // dx < spiralsize will lead to 
				spiralDirection = 2;              // change of direction to left
				spiralSize += pixelSize;
			};
			break;
		case 2: std::cout << "in case 2\n";
			if (y > ofGetWindowHeight()) {
				x = startX + spiralSize;
				spiralDirection = 0;
			}
			y++;
			if (y >= startY + spiralSize) {
				spiralDirection = 3; // change direction to left
			};
			break;
		case 3: std::cout << "in case 3\n";
			if (x > ofGetWindowWidth()) {
				y = startY - spiralSize;
				spiralDirection = 1;
			}
			x++;
			if (x >= startX + spiralSize) {
				spiralDirection = 0; // change direction to left
				spiralSize += pixelSize;
			};
			break;
	}
	if (x > windowWidth) {
		std::cout << "reseting spiral\n";
		x = startX;
		y = startY;
		spiralSize = pixelSize;
	}
}




