#include "PixelSizeDetector.h"

//_____________________________________________________________________________
void PixelSizeDetector::setup() {
	// set framerate
	ofSetFrameRate(20);
	// arbitrary value, gets actually "rounded" down to 48
	_noSquaresDrawn = 50;
	// pixelSize not found yet
	_foundPixelSize = false;
	// set maximal brightness to 0
	_maxBrightness = 0;

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
	_ratio = (float)_screenHeight / (float)_screenWidth;

	// calculate _maxPixelSize as the hundredth of the smaller screen size
	_maxPixelSize = (int) floor(min(_screenHeight, _screenWidth) / 100);
	ofBackground(ofColor::black);

	draw();
	_cam.update();
	_beginDetection = false;
	
	_img = _cam.getPixels();
	_imageHeight = _img.getHeight();
	_imageWidth = _img.getWidth();

	std::cout << "pixelSizeDetector done setting up\n";
}

//_____________________________________________________________________________
void PixelSizeDetector::update() {
	// static update of the received Signal
	_cam.update();
	// get image from camera
	_img = _cam.getPixels();
	_imgPixels = _img.getPixels();

	subtractBackground();

	detectBrightness();
	/*
	// if the seen screen is completly black (i.e. brightness < 10) begin square detection
	if (_maxBrightness < 10) {
		_beginDetection = true;
	}*/

	// Code for pixel size detection goes here
	if (_beginDetection == true) {
		_maxBrightness = 0;
		std::cout << "beginning pixel detection\n";

		//TODO: currently just a claim, need to verify!!
		// if the maximal found brightness is over 200 we have found our white square
		if (_maxBrightness >= 200) {
			_foundPixelSize = true;
		}
		else {
			// if not increase the pixelSize and better luck next time
			// needs to be += not ++, because of pointer/reference stuff
			_pixelSize += 1;
		}

		// if the pixel is found exit the app
		//if (_foundPixelSize == true) {
		//	ofGetWindowPtr()->setWindowShouldClose();
		//}
	}
}

//_____________________________________________________________________________
void PixelSizeDetector::draw() {
	std::cout << "beginning draw function\n";

	// calculates the number of square drawn on the screen
	int squaresWidth = floor(sqrt((float)_noSquaresDrawn / _ratio));
	int squaresHeight = floor(sqrt((float)_noSquaresDrawn * _ratio));

	// calculates the neccessary spacing between the spares
	// and the offset from (0, 0) position to get an even spacing
	int spacingWidth = _screenWidth / squaresWidth;
	int spacingHeight = _screenHeight / squaresHeight;
	int offsetWidth = spacingWidth / 2;
	int offsetHeight = spacingHeight / 2;
	
	// get a black background with white squares on them
	ofBackground(ofColor::black);
	ofSetColor(ofColor::white);
	ofFill();
	// actually draw the squares
	for (int i = 0; i < squaresWidth; i++) {
		for (int j = 0; j < squaresHeight; j++) {
			ofDrawRectangle(spacingWidth * i + offsetWidth, spacingHeight * j + offsetHeight, *_pixelSize, *_pixelSize);
		}
	}
	
	// debug
	_cam.draw(0, 0);
	_diff.draw(0, 240);
	ofImage img;
	img = _imgPixels;//_background;
	img.draw(0, 480);
	ofNoFill();

	ofSetColor(ofColor::red);
	ofDrawEllipse(_maxBrightnessX, _maxBrightnessY, 40, 40);

	ofSetColor(ofColor::black);
}


//_____________________________________________________________________________
void PixelSizeDetector::mousePressed(int x, int y, int button) {
	_cam.update();
	_background = _cam.getPixels();
	_beginDetection = true;
}

//_____________________________________________________________________________
void PixelSizeDetector::setPixelSizePointer(int *&pixelSize) {
	_pixelSize = pixelSize;
}

//_____________________________________________________________________________
void PixelSizeDetector::subtractBackground() {
	//ofxCvGrayscaleImage img;
	//_img.convertToGrayscalePlanarImage(img, 1);
	//CvMat mat = img.getCvImage().;


	if (_background.isAllocated()) {
		// iterate over all pixels and set the new colour for the difference image
		for (int y = 0; y < _imageHeight; y++) {
			for (int x = 0; x < _imageWidth; x++) {
				ofColor color = _imgPixels.getColor(x, y) - _background.getColor(x, y);
				_diff.setColor(x, y, color);
			}
		}
		_imgPixels = _diff.getPixels();
	}
	else {
		_diff = _img.getPixels();
	}

}

//_____________________________________________________________________________
void PixelSizeDetector::detectBrightness() {
	_maxBrightness = 0;
	std::cout << "beginning pixel detection\n";

	// iterate and find brightest pixel
	for (int y = 0; y < _imageHeight; y++) {
		for (int x = 0; x < _imageWidth; x++) {
			_colorAtXY = _imgPixels.getColor(x, y);
			_brightnessAtXY = _colorAtXY.getBrightness();
			if (_brightnessAtXY > _maxBrightness) {
				_maxBrightness = _brightnessAtXY;
				_maxBrightnessX = x;
				_maxBrightnessY = y;
				std::cout << "maxBrightness: " << _maxBrightness << "\n";
			}
		}
	}
}