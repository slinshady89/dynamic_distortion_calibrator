#include "PixelSizeDetector.h"

int WHITE_THRESHOLD = 100;

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

	_cam.update();
	
	// initial state is setup
	_state = -1;
	_drawCount = 0;
	_setupDone = false;

	_img = _cam.getPixels();
	_imageHeight = _img.getHeight();
	_imageWidth = _img.getWidth();

	_diff = _img.getPixels();

	_backgroundSet = false;

	_mousePressedOnce = false;
	_mousePressedTwice = false;

	_foundPixelSizeCounter = 2;

	std::cout << "pixelSizeDetector done setting up\n";
}

//_____________________________________________________________________________
void PixelSizeDetector::update() {

}

//_____________________________________________________________________________
void PixelSizeDetector::draw() {
	std::cout << "beginning draw function in state " << _state << " with pixelSize: " << *_pixelSize << "\n";
	
	bool debug = true;
	if (_state == -1) { // setup state
		// ensure that instructions don't mess with camera setup
		if (_mousePressedOnce == false) {
			ofClear(0);
			ofSetColor(ofColor::white);
			int posX = (int)round(((float)_screenWidth / 3.0));
			int posY = (int)round(((float)_screenHeight / 2.0));
			ofDrawBitmapString("Please set up your camera, so that it only 'sees' the screen and nothing else. \n", posX, posY - 10);
			ofDrawBitmapString("If you're done setting up your camera, right click once with your mouse.\n", posX, posY);
			ofDrawBitmapString("To begin, right click once with your mouse.", posX, posY + 10);
		} else {
			// draw the camera image on screen
			_cam.update();
			_img = _cam.getPixels();
			ofSetColor(ofColor::white);
			_img.draw(0, 0);

			// state gets set by mouse-click as well
		}
	}
	else if (_state == 0) { // drawing state
		// increase draw count
		_drawCount++;

		// draw the rectangles on the screen
		drawRectangles();

		// debug; draws the camera frame as seen, 
		if (debug == true) {
			drawDebug();
		}
		
		// count to ensure, that the new screen has actually been drawn before capturing it
		if (_drawCount == 10) {
			_drawCount = 0;
			_state = 1;
		}
	}
	else if (_state == 1) { // capturing frame state
		_cam.update();
		_img = _cam.getPixels();
		//_imgPixels = _img.getPixels();

		// if we're in this state for the first time, catch the background
		if (_backgroundSet == false) {
			std::cout << "allocating background \n";
			_background = _img.getPixels();
			_backgroundSet = true;
		}

		_state = 2;
	}
	else if (_state == 2) { // actual calculation state
		std::cout << "beginning pixel detection\n";
		// subtract the background
		_diffPixels = commonFunctions::subtractBackground(_img.getPixels(), _background);
		// and finally detect the position of the brightest pixel
		 _pos  bright = commonFunctions::detectBrightness(_diffPixels);
		_maxBrightnessX = bright.x;
		_maxBrightnessY = bright.y;
		_maxBrightness = bright.b;
		//TODO: currently just a claim, need to verify!!
		// if the maximal found brightness is over 100 we have found our white square
		if (_maxBrightness >= WHITE_THRESHOLD) {
			// if the pixelSize remained the same count it, else reset the counter and set new oldPixelSize
			if (_oldPixelSize == *_pixelSize) {
				_foundPixelSizeCounter++;
			}
			else {
				_oldPixelSize = *_pixelSize;
				_foundPixelSizeCounter = 0;
			}
			// if the same pixelSize was found 5 times, we assume the pixelSize was found
			if (_foundPixelSizeCounter == 5) {
				_foundPixelSize = true;
			}
		}
		else {
			// if not increase the pixelSize and better luck next time
			// needs to be += not ++, because of pointer/reference stuff
			if (_oldPixelSize == 0) { // repeat pixelSize = 1 once
				_oldPixelSize = *_pixelSize;
				*_pixelSize = 1;
			}
			else {
				_oldPixelSize = *_pixelSize;
				*_pixelSize += 1;
			}
		}

		// if the pixel is found exit the app
		if (_foundPixelSize == true) {
			std::cout << "found pixelSize = " << *_pixelSize << "\n";
			ofGetWindowPtr()->setWindowShouldClose();
		}

		// go back to drawing state
		_state = 0;
	}
	else { // case if something goes wrong
		std::cout << "Something went wrong, unreachable state reached. Quitting now.\n";
		ofGetWindowPtr()->setWindowShouldClose();
	}
}


//_____________________________________________________________________________
void PixelSizeDetector::mousePressed(int x, int y, int button) {
	if (_mousePressedOnce == false) {
		_mousePressedOnce = true;
	}
	else if (_mousePressedOnce == true && _mousePressedTwice == false) {
		// ensure that background gets set only once and we don't trigger anything
		// on a third click
		_setupDone = true;
		_mousePressedTwice = true;
		_state = 0;
	}
}

//_____________________________________________________________________________
void PixelSizeDetector::setPixelSizePointer(int *&pixelSize) {
	_pixelSize = pixelSize;
}

//_____________________________________________________________________________
void PixelSizeDetector::drawRectangles()
{
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
}

//_____________________________________________________________________________
void PixelSizeDetector::drawDebug()
{
	ofImage img;
	img = _diffPixels;// difference between _cam;
	img.draw(0, 0);
	_cam.draw(0, 240);
	ofImage background;
	background = _background;
	background.draw(0, 480);
	
	ofNoFill();

	ofSetColor(ofColor::red);
	ofDrawEllipse(_maxBrightnessX, _maxBrightnessY, 40, 40);
	ofFill();
	ofSetColor(ofColor::black);
}