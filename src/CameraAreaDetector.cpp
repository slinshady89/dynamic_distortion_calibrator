#include "CameraAreaDetector.h"

//_____________________________________________________________________________
void CameraAreaDetector::setup()
{
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

	std::cout << "_screenHeight " << _screenHeight << " _screenWidth " << _screenWidth << "\n";

	_cam.update();

	// get image dimensions
	_img = _cam.getPixels();
	_imgPixels = _img.getPixels();
	_imageHeight = _img.getHeight();
	_imageWidth = _img.getWidth();
	_area->_sizeImageX = _imageWidth;
	_area->_sizeImageY = _imageHeight;
	
	// allocate storage for the screen coordinates
	_area->_imageX = new int*[_imageWidth];
	_area->_imageY = new int*[_imageWidth];
	for (int i = 0; i < _imageWidth; i++) {
		_area->_imageX[i] = new int[_imageHeight];
		_area->_imageY[i] = new int[_imageHeight];
	}

	// set all values to -1 in order to later be able to easier identify seen places
	for (int y = 0; y < _imageHeight; y++) {
		for (int x = 0; x < _imageWidth; x++) {
			_area->_imageX[x][y] = -1;
			_area->_imageY[x][y] = -1;
		}
	}

	// start drawing at middle of screen
	_screenX = _screenWidth / 2;
	_screenY = _screenHeight / 2;

	ofBackground(ofColor::black);
	ofSetColor(ofColor::white);

	_state = -1;
	_drawCount = 0;
	_maxBrightness = 0;

	_backgroundSet = false;
	_background = _img.getPixels();
	_vis.allocate(_screenWidth, _screenHeight, OF_IMAGE_COLOR);
	std::cout << "_vis height " << _vis.getHeight() << " width" << _vis.getWidth()<< "\n";
	for (int x = 0; x < _vis.getWidth(); x++) {
		for (int y = 0; y < _vis.getHeight(); y++) {
			_vis.setColor(x, y, ofColor::black);
		}
	}
	_visDrawn = false;
}

//_____________________________________________________________________________
void CameraAreaDetector::update()
{
	
}

//_____________________________________________________________________________
void CameraAreaDetector::draw()
{
	bool debug = true;

	if (_state == -1) {
		_drawCount++;
		
		_cam.update();
		_img = _cam.getPixels();
		drawDebug();
		if (_drawCount == 50) {
			_drawCount = 0;
			_cam.update();
			_backgroundSet = true;
			_background = _cam.getPixels();
			_state = 0;
		}
	} else if (_state == 0) {
		// entered drawing state
		_drawCount++;

		if (_visDrawn == false) {
			ofImage vis;
			vis = _vis;
			vis.draw(0, 0);
			_visDrawn = true;
		}

		// debug; draws the camera frame as seen, 
		if (debug == true) {
			drawDebug();
		}

		ofDrawRectangle(_screenX, _screenY, _pixelSize, _pixelSize);

		// next call capturing state
		if (_drawCount == 10) {
			_drawCount = 0;
			_visDrawn = false;
			_state = 1;
		}

	} else if(_state == 1) {
		// entered capturing state

		// static update of camera
		_cam.update();

		// get image from camera
		_img = _cam.getPixels();
		_imgPixels = _img.getPixels();

		// next call calculation state
		_state = 2;
	}
	else if (_state == 2) {
		// entered calculation state

		determineAndSetPosition();

		calculateNextPosition();

		if (allPlacesSeen()) {
			ofGetWindowPtr()->setWindowShouldClose();
		}
		
		// next call drawing state
		_state = 0;
	}
	else {
		// something went wrong
		std::cout << "Something went wrong, unreachable state reached. Quitting now.\n";
		ofGetWindowPtr()->setWindowShouldClose();
	}
}

void CameraAreaDetector::mousePressed(int x, int y, int button) {
	
}

//_____________________________________________________________________________
void CameraAreaDetector::setCameraAreaPointerAndPixelSize(cameraArea *& area, int pixelSize)
{	
	_pixelSize = pixelSize;
	_area = area;
}

//_____________________________________________________________________________
void CameraAreaDetector::drawDebug()
{
	ofImage img;
	img = _diffPixels;// difference between _cam;
	img.draw(0, 0);
	_cam.draw(0, 240);
	ofImage background;
	background = _background;
	background.draw(0, 480);

	ofNoFill();
	if (_maxBrightness > 130) {
		ofSetColor(ofColor::red);
		ofDrawEllipse(_maxBrightnessX, _maxBrightnessY, 40, 40);
		ofSetColor(ofColor::white);
	}
}

//_____________________________________________________________________________
void CameraAreaDetector::determineAndSetPosition()
{
	// subtract the background
	_diffPixels = commonFunctions::subtractBackground(_img.getPixels(), _background);
	// and finally detect the position of the brightest pixel
	tuple<int, int, float> bright = commonFunctions::detectBrightness(_diffPixels);
	_maxBrightnessX = std::get<0>(bright);
	_maxBrightnessY = std::get<1>(bright);
	_maxBrightness = std::get<2>(bright);
	std::cout << "max brightness: " << _maxBrightness << "\n";
	// assumed as white
	if (_maxBrightness > 130) {
		_area->_imageX[_maxBrightnessX][_maxBrightnessY] = _screenX;
		_area->_imageY[_maxBrightnessX][_maxBrightnessY] = _screenY;
		_vis.setColor((size_t)_screenX, (size_t)_screenY, ofColor::darkOliveGreen);
	}
}

//_____________________________________________________________________________
void CameraAreaDetector::calculateNextPosition()
{
	_screenX++;
	if (_screenX > _screenWidth) {
		_screenX = 0;
		_screenY++;
	}
	if (_screenY > _screenHeight) {
		_screenY = 0;
	}
}

//_____________________________________________________________________________
bool CameraAreaDetector::allPlacesSeen() {
	for (int y = 0; y < _imageHeight; y++) {
		for (int x = 0; x < _imageWidth; x++) {
			// only need to look at either x or y as they get set at the same time
			if (_area->_imageX[x][y] == -1) {
				return false;
			}
		}
	}
	return true;
}