#include "CameraBorderDetector.h"

//_____________________________________________________________________________
void CameraBorderDetector::setup()
{
	// set framerate
	ofSetFrameRate(20);
	_nonVisCount = 0;
	pxSeenOnce = false;
	// check for external web cam and use that if possible
	if (_cam.listDevices().size() > 1) {
		_cam.setDeviceID(1);
	}
	else {
		_cam.setDeviceID(0);
	}
	// setup camera
	_cam.setup(320, 240);

	// get screen dimensions
	_screenHeight =  ofGetWindowHeight();
	_screenWidth =  ofGetWindowWidth();

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
  _screenX = 0; // _screenWidth / 2;
	_screenY = 0; // _screenHeight / 2;

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


	_pixelSeen = true;
	_borderDetected = false;
}

//_____________________________________________________________________________
void CameraBorderDetector::update()
{
	
}

//_____________________________________________________________________________
void CameraBorderDetector::draw()
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
      if (!_borderDetected) {
        binarySearch();
      }
      else {
        // find borders
      }
			if (allPlacesSeen()) {
				ofGetWindowPtr()->setWindowShouldClose();
			}
		}
		// next call drawing state
		_state = 0;
	}
	else {
		// something went wrong
		cout << "Something went wrong, unreachable state reached. Quitting now.\n";
		ofGetWindowPtr()->setWindowShouldClose();
	}
}

void CameraBorderDetector::mousePressed(int x, int y, int button) {
	
}

//_____________________________________________________________________________
void CameraBorderDetector::setCameraAreaPointerAndPixelSize(cameraArea *& area, int pixelSize)
{	
	_pixelSize = pixelSize;
	_area = area;
}

//_____________________________________________________________________________
void CameraBorderDetector::drawDebug()
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
void CameraBorderDetector::determineAndSetPosition()
{
	// subtract the background
	_diffPixels = commonFunctions::subtractBackground(_img.getPixels(), _background);
	// and finally detect the position of the brightest pixel
	_pos bright = commonFunctions::detectBrightness(_diffPixels);
  _maxBrightnessX = bright.x;
  _maxBrightnessY = bright.y;
  _maxBrightness = bright.b;
	cout << "max brightness: " << _maxBrightness << "\n";
	// assumed as white
  if (_maxBrightness > 130) {
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

      //cout << "_screen " << _screenX << " " << _screenY  << endl;
      //cout << "_lastSeenPos "<< bright.x << " " << bright.y << " " << bright.b << endl;
      _nonVisCount = 0;
    }
    _pixelSeen = true;
  }
  else {
    _pixelSeen = false;
  }
}

//_____________________________________________________________________________
void CameraBorderDetector::binarySearch()
{
	// detection of a the right border of the camera frame
	
	if (!_borderDetected) {
		// binary search algorithm
		if (_pixelSeen) {
      _lastSeen = _drawnPos;
		}
		else {
      _lastNotSeen = _drawnPos;
    }
    cout << "_lastSeen " << _lastSeen.x << " " << _lastSeen.y << endl;
    cout << "_lastNotSeen " << _lastNotSeen.x << " " << _lastNotSeen.y << endl;

    _screenX = ceil(((float)_lastSeen.x + _lastNotSeen.x) / 2.0);

    cout << "screen " << _screenX << " " << _screenY << endl;

    // height should stay at the same 
		//_screenY = (_lastSeen.y + _lastNotSeen.y) / 2;
    _drawnPos.x = _screenX; _drawnPos.y = _screenY;
    cout << "actual pos: " << _screenX << " " << _screenY  << endl;
	}
  if ((_drawnPos.x == _lastSeen.x) && (_drawnPos.y == _lastSeen.y)) {
    _borderDetected = true;
  }
}



//_____________________________________________________________________________
bool CameraBorderDetector::allPlacesSeen() {
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

void CameraBorderDetector::findInitialPosition() {
  int spacing = _screenHeight / 5; //to get at least 10 different rows of pixels
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

  // setup for binarySearch()
  if (_initPos) {
    _lastSeen.x = _screenX; _lastSeen.y = _screenY;
    cout << "_lastSeen " << _lastSeen.x << " " << _lastSeen.y << endl;
    _drawnPos = _lastSeen;
    _lastNotSeen.x = _screenWidth; _lastNotSeen.y = _screenY;
  }
}

void CameraBorderDetector::borderWalker()
{
}

