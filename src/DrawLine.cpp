#include "DrawLine.h"

int WHITE_THRESHOLD = 100;

//_____________________________________________________________________________
void DrawLine::setup() {
	// set framerate
	ofSetFrameRate(20);
	
	// check for external webcam and use taht if possible
	if (_cam.listDevices().size() > 1) {
		_cam.setDeviceID(1);
	}
	else {
		_cam.setDeviceID(0);
	}
	// setup camera
	_cam.setup(320, 240);

	// get screen dimensions
	_screenHeight = ofGetWindowHeight();
	_screenWidth = ofGetWindowWidth();

	ofBackground(ofColor::black);

	_cam.update();

	ofSetBackgroundAuto(false);

	_img = _cam.getPixels();
	_imageHeight = _img.getHeight();
	_imageWidth = _img.getWidth();

	_diff = _img.getPixels();

}

//_____________________________________________________________________________
void DrawLine::update() {

}

//_____________________________________________________________________________
void DrawLine::draw() {

	_cam.update();

	bool debug = true;

	// debug; draws the camera frame as seen, 
	if (debug == true) {
		drawDebug();
	}

	// take image and convert it to grayscale
	_color.setFromPixels(_cam.getPixels());
	_img = _color;

	// if we're in this state for the first time, catch the background
	if (_backgroundSet == false) {
		std::cout << "allocating background \n";
		_background = _img.getPixels();
		_backgroundSet = true;
	}

	drawRectangles();

	// take image and convert it to grayscale
	_color.setFromPixels(_cam.getPixels());
	_img = _color;

	ofxCvGrayscaleImage diff;
	diff.setFromPixels(_background);
	diff.absDiff(_img);
	_diffPixels = diff.getPixels();

	// from ofxCvGrayScaleImage to cvArr*
	cv::Mat mat = diff.getCvImage();
	IplImage img = mat;
	IplImage edges;
	cvCanny(&img, &edges, 150, 380, 3);
	// from cvArr* to image
	*_contour.getCvImage() = edges;
}


//_____________________________________________________________________________
void DrawLine::drawRectangles()
{

	// get a black background with white squares on them
	ofBackground(ofColor::black);
	ofSetColor(ofColor::white);
	ofFill();
	// actually draw the squares
	ofDrawRectangle(0,0,_screenWidth/2, _screenHeight);
}

//_____________________________________________________________________________
void DrawLine::drawDebug()
{
	ofImage img;
	img = _contour;// difference between _cam;
	img.draw(0, 0);
	_cam.draw(0, 240);
	ofImage background;
	background = _background;
	background.draw(0, 480);

	ofNoFill();

}