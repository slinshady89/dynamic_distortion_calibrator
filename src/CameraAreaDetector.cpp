#include "CameraAreaDetector.h"

//_____________________________________________________________________________
void CameraAreaDetector::setup()
{
	// set framerate
	ofSetFrameRate(20);
	_nonVisCount = 0;
	pxSeenOnce = false;
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
  _screenHeight = 0; // ofGetWindowHeight();
	_screenWidth = 0; // ofGetWindowWidth();

	std::cout << "_screenHeight " << _screenHeight << " _screenWidth " << _screenWidth << "\n";

	_cam.update();

	// get image dimensions
	_img = _cam.getPixels();
	_imgPixels = _img.getPixels();
	_imageHeight = _img.getHeight();
	_imageWidth = _img.getWidth();
	_area->_sizeImageX = _imageWidth;
	_area->_sizeImageY = _imageHeight;
	

	_minX = make_tuple (_imageWidth,0);
	_minY = make_tuple(0,_imageHeight);
	_maxX = make_tuple(0,0);
	_maxY = make_tuple(0,0);


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
	_initPos = false;
	_cumulativeX = 0;
	_cumulativeY = 0;
	_seenCount = 0;
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

		ofFill();
		ofDrawRectangle(_screenX, _screenY, _pixelSize, _pixelSize);

		// next call capturing state
		if (_drawCount == 4) {  // minimum 4 for Nils' pc check all if you need higher timer!
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
		if (_initPos == false) {
			determineAndSetPosition();

			// find next square position
			findInitialPosition();
		} else {
			determineAndSetPosition();

			calculateNextPosition();

			if (allPlacesSeen()) {
				ofGetWindowPtr()->setWindowShouldClose();
			}
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
	if (_maxBrightness > 100) {
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
	if (_maxBrightness > 100) {
		if (_initPos == false) {
			_cumulativeX += _screenX;
			_cumulativeY += _screenY;
			_seenCount++;
		}
		else {
			_area->_imageX[_maxBrightnessX][_maxBrightnessY] = _screenX;
			_area->_imageY[_maxBrightnessX][_maxBrightnessY] = _screenY;
			_vis.setColor((size_t)_screenX, (size_t)_screenY, ofColor::darkOliveGreen);
			_lastSeenPos = bright;
			_nonVisCount = 0;
		}
	}
}

//_____________________________________________________________________________
void CameraAreaDetector::calculateNextPosition()
{
	if (false) {
		_screenX++;
		if (_screenX > _screenWidth) {
			_screenX = 0;
			_screenY++;
		}
		if (_screenY > _screenHeight) {
			_screenY = 0;
		}
	}
	else {// spiral
		switch (_dirMovement) {
		case 0:
			if (_screenY < 0) {                        // reaching upper windowborder
				_screenX = _startX - _spiralSize;          // change x to left spiral side
				_dirMovement = 2;              // -> 180° turn of direction
			}
			_screenY--;
			if (_screenY <= _startY - _spiralSize) {     // dy > _spiralSize will lead to
				_dirMovement = 1;              // change of direction to left
			};
			break;
		case 1:
			if (_screenX < 0) {                        // reaching left windowborder
				_screenY = _startY + _spiralSize;          // change x to upper spiral side
				_dirMovement = 3;              // -> 180° turn of direction
			}
			_screenX--;
			if (_screenX <= _startX - _spiralSize) {     // dx < _spiralSize will lead to
				_dirMovement = 2;              // change of direction to left
				_spiralSize++;
			};
			break;
		case 2:
			if (_screenY > ofGetWindowHeight()) {
				_screenX = _startX + _spiralSize;
				_dirMovement = 0;
			}
			_screenY++;
			if (_screenY >= _startY + _spiralSize) {
				_dirMovement = 3; // change direction to left
			};
			break;
		case 3:
			if (_screenX > ofGetWindowWidth()) {
				_screenY = _startY - _spiralSize;
				_dirMovement = 1;
			}
			_screenX++;
			if (_screenX >= _startX + _spiralSize) {
				_dirMovement = 0; // change direction to left
				_spiralSize++;
			};
			break;
		}
		if (_screenX > _screenWidth) {
			std::cout << "reseting spiral\n";
			_screenX = _startX;
			_screenY = _startY;
			_spiralSize = 1;
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

void CameraAreaDetector::findInitialPosition() {
	int spacing = 100; //arbitratily chosen
	_screenX += spacing;
	if (_screenX > _screenWidth) {
		_screenX = 0;
		_screenY += spacing;
	}
	if (_screenY > _screenHeight) {
		_startX = ceil((float)_cumulativeX / (float)_seenCount);
		_startY = ceil((float)_cumulativeY / (float)_seenCount);
		_screenX = _startX;
		_screenY = _startY;
		_initPos = true;
	}
}

void CameraAreaDetector::detectBordersOfFrame() {
	for (int x = 0; x < _vis.getWidth(); x++) {
		for (int y = 0; y < _vis.getHeight(); y++) {			
			// if a pixel is detected the brightness should be higher than 1
			if (_vis.getColor(x, y).getBrightness() > 1) {
				if (y <= get<1>(_minY)) {
					_minY = make_tuple(x, y);
				}
				else if (x <= get<0>(_minX)) {
					_minX = make_tuple(x, y);
				}
				else if (y >= get<1>(_maxY)) {
					_maxY = make_tuple(x, y);
				}
				else if (x >= get<0>(_maxX)) {
					_maxX = make_tuple(x, y);
				}
			}
		}
	}
}