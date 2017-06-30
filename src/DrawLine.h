#pragma once

#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofMain.h"
#include "commonFunctions.h"
//#include <ofxCv>

class DrawLine : public ofBaseApp {
public:
	// ofBaseApp's setup function
	void setup();
	// ofBaseApp's draw function, gets looped internally by ofRunApp(...)
	void draw();
	// ofBaseApp's update functin, gets looped internall by ofRunApp(...)
	// currently not actually used in this code
	void update();

private:
	// screenHeight
	int _screenHeight;
	// screenWidth
	int _screenWidth;
	// imageHeight
	int _imageHeight;
	// imageWidth
	int _imageWidth;

	// keep track of whether the background has already been set
	bool _backgroundSet;

	
	// Object for camera signal
	ofVideoGrabber _cam;
	// color image taken from camera grabber
	ofxCvColorImage _color;
	// grayscale image from camera
	ofxCvGrayscaleImage _img;
	// pixel array of the image
	ofPixels _imgPixels;

	// background taken at beginning of pixel detection
	ofPixels _background;
	// difference between image and background
	ofxCvGrayscaleImage _diff;
	// pixel array of the difference image
	ofPixels _diffPixels;
	// image of detected line
	ofImage _contour;
	
	// function that draws the pattern
	void drawRectangles();

	// draws the current camera frame, the set background, & the remaining differenece
	// in the upper left corner if _debug is set true
	void drawDebug();
};