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

	ofBackground(ofColor::black);

	draw();
	_cam.update();

	// get image dimensions
	_img = _cam.getPixels();
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

	// start drawing at (0, 0)
	_screenX = 0;
	_screenY = 0;

	_beginDetection = false;

	_vis.allocate(_screenWidth, _screenHeight, OF_IMAGE_COLOR);
	_vis.setColor(ofColor::black);
}

//_____________________________________________________________________________
void CameraAreaDetector::update()
{
	// static update of camera
	_cam.update();

	// get image from camera
	_img = _cam.getPixels();
	_imgPixels = _img.getPixels();

	//TODO add in background subtraction once it works

	// detect brightestPixel
	detectBrightness();

	// assumed as white
	if (_maxBrightness > 200) {
		_area->_imageX[_maxBrightnessX][_maxBrightnessY] = _screenX;
		_area->_imageY[_maxBrightnessX][_maxBrightnessY] = _screenY;
		_vis.setColor(_screenX, _screenY, ofColor::darkBlue);
	}

	if (allPlacesSeen()) {
		ofGetWindowPtr()->setWindowShouldClose();
	}
}

//_____________________________________________________________________________
void CameraAreaDetector::draw()
{	
	//_vis.draw(0, 0);
	//ofColor::white;
	ofDrawRectangle(_screenX, _screenY, _pixelSize, _pixelSize);
	//ofColor::black;

	// move the pixel along the screen
	if (_beginDetection) {
		_screenX++;
		if (_screenX == _screenWidth) {
			_screenX = 0;
			_screenY++;
		}
		if (_screenY == _screenHeight) {
			_screenX = 0;
			_screenY = 0;
		}
	}

	// for debug purposes; allows to adjust the camera to see screen only
	if (!_beginDetection) {
		_img.draw(0, 0);
	}

}

void CameraAreaDetector::mousePressed(int x, int y, int button) {
	_beginDetection = true;
}

//_____________________________________________________________________________
void CameraAreaDetector::setCameraAreaPointerAndPixelSize(cameraArea *& area, int pixelSize)
{	
	_pixelSize = pixelSize;
	_area = area;
}

//_____________________________________________________________________________
void CameraAreaDetector::detectBrightness() {
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