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
	_border->_sizeImageX = _imageWidth;
	_border->_sizeImageY = _imageHeight;
	
	// allocate storage for the screen coordinates
	_border->_borderArray = new pos*[_screenWidth];
	for (int i = 0; i < _screenWidth; i++) {
		_border->_borderArray[i] = new pos[_screenHeight];
	}

	// set all values to -1 in order to later be able to easier identify seen places
	for (int y = 0; y < _screenHeight; y++) {
		for (int x = 0; x < _screenWidth; x++) {
			_border->_borderArray[x][y].x = -1;
			_border->_borderArray[x][y].y = -1;
		}
	}

	// start drawing at middle of screen
	_screen.x = 0; // _screenWidth / 2;
	_screen.y = 0; // _screenHeight / 2;

	ofSetBackgroundAuto(false);
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
	_initPosFound = false;
	_cumulativeX = 0;
	_cumulativeY = 0;
	_seenCount = 0;

	_firstBinarySearchCall = true;
	_binarySearchDirection = 0;

	_pixelSeen = true;
	_borderDetected = false;

	// new from here on
	_borderState = 0;

	// initialize ground truths (same size)
	/*_vertical->_groundTruth = new pos*[_screenWidth];
	_horizontal->_groundTruth = new pos*[_screenWidth];

	for (int i = 0; i < _screenWidth; i++) {
		_vertical->_groundTruth[i] = new pos[_screenHeight];
		_horizontal->_groundTruth[i] = new pos[_screenHeight];
	}

	for (int x = 0; x < _screenWidth; x++) {
		for (int y = 0; y > _screenHeight; y++) {
			_vertical->groundTruth[x][y].x = -1;
			_horizontal->groundTruth[x][y].x = -1;
		}
	}

	// initialize image
	_vertical->image = new pos*[_imageWidth];
	_horizontal->image = new pos*[_imageWidth];

	for (int i = 0; i < _imageWidth; i++) {
		_vertical->image[i] = new pos[_imageHeight];
		_horizontal->image[i] = new pos[_imageHeight];
	}

	for (int x = 0; x < _imageWidth; x++) {
		for (int y = 0; y > _imageHeight; y++) {
			_vertical->image[x][y].x = -1;
			_horizontal->image[x][y].x = -1;
		}
	}*/

	_vertical->_verticalOffsets = new int[_imageHeight];
	_horizontal->_horizontalOffsets = new int[_imageWidth];
	//test: _border->_borderArray[0][0].x = 100;
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
		// wait 50 iterations of drawing the first black background until the camera and all objects are loaded
		// switch to state 0 if these 50 iterations are done
		if (_drawCount == 50) {
			_drawCount = 0;
			_cam.update();
			_backgroundSet = true;
			_color = _cam.getPixels();
			_img = _color;
			_background = _img.getPixels();
			_state = 0;
		}
	}
	else if (_state == 0) {
		// entered drawing state
		_drawCount++;
		// initialize _vis object to local object for drawing some dark green pixels on it for debug purposes 
		ofImage vis;
		vis = _vis;
		vis.draw(0, 0);

		// debug; draws the camera frame as seen, 
		if (debug == true) {
			drawDebug();
		}
		cout << "_state 0, _borderstate " << _borderState << "\n";
		ofSetColor(ofColor::white);
		ofFill();
		if(_borderState == 0) {
			// actually draws the pixel that should be detected for detection the borders of the camera frame
			ofDrawRectangle(_screen.x, _screen.y, _pixelSize, _pixelSize);
		}
		else if (_borderState == 1) {
			ofDrawRectangle(_initPos.x, 0, _pixelSize, _screenHeight);
		}
		else if (_borderState == 2) {
			ofDrawRectangle(0, _initPos.y, _screenWidth, _pixelSize);
		}
		else if (_borderState == 3) {
			makeVerticalLines();
		}
		else if (_borderState == 4) {
			makeHorizontalLines();
		}
		else if (_borderState == 5) {
			_img.draw(_initPos.x, _initPos.y);
			
			//fillOutBorders();
		}

		// next call capturing state
		if (_drawCount == 5) {  // minimum 4 for Nils' computer check all if you need higher timer!
			_drawCount = 0;
			_visDrawn = false;
			_state = 1;
		}

	} else if(_state == 1) {
		cout << "_state 0, _borderstate " << _borderState << "\n";
		// entered capturing state

		// static update of camera
		_cam.update();

		// get image from camera, conversion happens implicitly
		_color = _cam.getPixels();
		_img = _color;
		_imgPixels = _img.getPixels();

		// next call calculation state
		_state = 2;
	}
	else if (_state == 2) {
		// entered calculation state
		/*if (_initPosFound == false) {
			determineAndSetPosition();
			// find next square position
			findInitialPosition();

		} else {
			// determineAndSetPosition();
			// detectBorder();

			makeInitCross();

			if (allPlacesSeen()) {
				ofGetWindowPtr()->setWindowShouldClose();
			}
		}*/
		cout << "_state 0, _borderstate " << _borderState << "\n";
		if (_borderState == 0) {
			determineAndSetPosition();
			findInitialPosition();
			if (_initPosFound == true) {
				_borderState = 1;
			}
		}
		else if (_borderState == 1 || _borderState == 2) {
			bool vertical = false;
			if (_borderState == 1) {
				vertical = true;
			}
			writeInitialCross(vertical);
			_borderState++;
		}
		else if (_borderState == 3) {
			writeVerticalLines();
			_borderState = 4;
		}
		else if (_borderState == 4) {
			writeHorizontalLines();
			calculateBorders();
			_borderState = 5;
		}
		else if (_borderState == 5) {

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

//_____________________________________________________________________________
void CameraBorderDetector::setCameraAreaPointerAndPixelSize(cameraBorder *& area, int pixelSize)
{	
	_pixelSize = pixelSize;
	_border = area;
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
	if (_maxBrightness > WHITE_THRESHOLD) {
		ofSetColor(ofColor::red);
		ofDrawEllipse(_maxBrightnessX, _maxBrightnessY, 40, 40);
		ofSetColor(ofColor::white);
	}
}

//_____________________________________________________________________________
void CameraBorderDetector::determineAndSetPosition()
{
	// subtract the background
	//_diffPixels = commonFunctions::subtractBackground(_img.getPixels(), _background);
	
	ofxCvGrayscaleImage diff;
	diff.setFromPixels(_background);
	diff.absDiff(_img);
	_diffPixels = diff.getPixels();

	// and finally detect the position of the brightest pixel in the image
	pos bright = commonFunctions::detectGrayValue(_diffPixels);
	cout << "max brightness: " << bright.b << "\n";
	// assumed as white
	if (bright.b > WHITE_THRESHOLD) {
		cout << "seen pixel \n";
		if (_initPosFound == false) {
			_cumulativeX += _screen.x;
			_cumulativeY += _screen.y;
			_seenCount++;
		}
		else {
			//_vis.setColor((size_t)_screen.x, (size_t)_screen.y, ofColor::darkOliveGreen);
			_lastSeenPos = bright;
			_lastSeen = _screen;
			_nonVisCount = 0;
		}
		_pixelSeen = true;
	}
	else {
		cout << "not seen pixel \n";
		_pixelSeen = false;
		cout << "_vis size: " << _vis.getWidth() << " x " << _vis.getHeight() << " versus position: " << _screen.x << ", " << _screen.y << "\n";
		//_vis.setColor((size_t)_screen.x, (size_t)_screen.y, ofColor::darkOrchid);
	}
  
}

//_____________________________________________________________________________
void CameraBorderDetector::findInitialPosition() {
  int spacing = _screenHeight / 6; //to get at least 10 different rows of pixels
  _screen.x += spacing;
  if (_screen.x > _screenWidth) {
    _screen.x = 0;
    _screen.y += spacing;
  }
  if (_screen.y > _screenHeight) {
    _initPos.x = ceil((float)_cumulativeX / (float)_seenCount);
    _initPos.y = ceil((float)_cumulativeY / (float)_seenCount);
    _screen.x = _initPos.x;
    _screen.y = _initPos.y;
    _initPosFound = true;
  }
  // setup for binarySearch()
 /* if (_initPosFound == true) {
    _lastSeen.x = _screen.x;
	_lastSeen.y = _screen.y;
    _lastNotSeen.x = _screenWidth; 
	_lastNotSeen.y = _screen.y;
    _drawnPos = _lastNotSeen;
	_screen = _lastNotSeen;
    cout << "found initial posX " << _screen.x << " posY " << _screen.y <<endl;
  }*/
}

//_____________________________________________________________________________
void CameraBorderDetector::makeInitialCross() {
	ofDrawRectangle(_initPos.x, 0, _pixelSize, _screenHeight);
	ofDrawRectangle(0, _initPos.y, _screenWidth, _pixelSize);
}

//_____________________________________________________________________________
void CameraBorderDetector::writeInitialCross(bool vertical) {
	unsigned char* pixelData = _imgPixels.getPixels();
	cv::Mat mat;
	mat = _img.getCvImage();
	for (int x = 0; x < _imageWidth; x++) {
		for (int y = 0; y > _imageHeight; y++) {
			if (pixelData[_imgPixels.getPixelIndex(x, y)] > WHITE_THRESHOLD) {
				if (vertical) {
					// we walk from left to right through the image
					// the first time we encounter a white pixel, it's part
					// of the initial cross line and thus yields the offset to
					// split the image into halves
					_vertical->_verticalOffsets[y] = x;
					// wherever in the image the first vertical line is seen
					// in the screen it's x position hasn't changed from the
					// initial value
					_vertical->_matchX.at<uchar>(x, y) = _initPos.x;
				}
				else {
					_horizontal->_horizontalOffsets[x] = y;
					_horizontal->_matchY.at<uchar>(x, y) = _initPos.y;
					// if the vertical offset and the horizontal offset match
					// this point is the initial point and now we know for a
					// single point, both camera and screen coordinates
					if (_vertical->_verticalOffsets[y] == x) {
						_horizontal->_initialPosition.x = _initPos.x;
						_horizontal->_initialPosition.y = _initPos.y;
						_vertical->_initialPosition.x = _initPos.x;
						_vertical->_initialPosition.y = _initPos.y;
					}
				}
			}
		}
	}
}

//_____________________________________________________________________________
void CameraBorderDetector::makeHorizontalLines() {
	// image is roughly halved by initial lines
	int heightUp = _initPos.y;
	int heightDown = _screenHeight - _initPos.y;

	// calculate how many lines go above and below initial line
	int noLinesUp = ceil(heightUp / (float)_pixelSize);
	int noLinesDown = ceil(heightDown / (float)_pixelSize);

	// actually draw lines
	for (int i = 0; i < noLinesUp;) {
		ofDrawRectangle(0, i * _pixelSize, _screenWidth, _pixelSize);
		i += 2;
	}

	for (int i = 1; i < noLinesUp;) {
		ofDrawRectangle(0, i * _pixelSize + _initPos.y, _screenWidth, _pixelSize);
		i += 2;
	}
}

//_____________________________________________________________________________
void CameraBorderDetector::writeHorizontalLines() {
	_horizontal->_image = _img.getCvImage();

}

//_____________________________________________________________________________
void CameraBorderDetector::makeVerticalLines() {
	// image is roughly halved by initial lines
	int widthLeft = _initPos.x;
	int widthRight = _screenWidth - _initPos.x;

	// calculate how many lines go above and below initial line
	int noLinesLeft = ceil(widthLeft / (float)_pixelSize);
	int noLinesRight = ceil(widthRight / (float)_pixelSize);

	// actually draw lines
	for (int i = 0; i < noLinesLeft;) {
		ofDrawRectangle(i * _pixelSize, 0, _pixelSize, _screenHeight);
		i += 2;
	}

	for (int i = 1; i < noLinesRight;) {
		ofDrawRectangle(i * _pixelSize + _initPos.x, 0, _pixelSize, _screenHeight);
		i += 2;
	}
}

//_____________________________________________________________________________
void CameraBorderDetector::writeVerticalLines() {
	_vertical->_image = _img.getCvImage();
}

//_____________________________________________________________________________
void CameraBorderDetector::calculateBorders() {
	cv::Mat vert = _vertical->_image;
	cv::Mat hor = _horizontal->_image;
	pos init = _horizontal->_initialPosition;

	EdgeDetector ed;

	ed.computeSobel(vert);
	cv::Mat vertEdges = ed.getSobelImage();

	ed.computeSobel(hor);
	cv::Mat horEdges = ed.getSobelImage();
	
	IplImage iplimg = hor;
	*_img.getCvImage() = iplimg;

	// find left border
	for (int y = 0; y < _imageHeight; y++) {
		int x = _vertical->_verticalOffsets[y];
	}
}

//_____________________________________________________________________________
void CameraBorderDetector::fillOutBorders() {

}


//_____________________________________________________________________________
void CameraBorderDetector::setImageReturnVariables(calibrationImage *&vertical, calibrationImage *&horizontal)
{
	_vertical = vertical;
	_horizontal = horizontal;
}


/*
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
	return false;
}

//_____________________________________________________________________________
void CameraBorderDetector::binarySearch(int dir, pos lastNotSeen, bool init)
{
	// sets initial lastNotSeen position if necessary
	if (init == true) {
		_lastNotSeen = lastNotSeen;
	}

	// if the currently drawn pixel was seen, set lastSeen, else lastNotSeen
	if (_pixelSeen == true) {
		_lastSeen.x = _screen.x;
		_lastSeen.y = _screen.y;
	}
	else {
		_lastNotSeen.x = _screen.x;
		_lastNotSeen.y = _screen.y;
	}

	cout << "_lastSeen " << _lastSeen.x << " " << _lastSeen.y << endl;
	cout << "_lastNotSeen " << _lastNotSeen.x << " " << _lastNotSeen.y << endl;

	_binarySearchDirection;

	// average over the lastSeen and lastNotSeen pixel is next position to consider
	// ceil and floor depend on search direction, as we want to keep the last seen
	// pixel as the border pixel.
	if (dir == 0 || dir == 3) {
		_screen.x = ceil(((float)_lastSeen.x + _lastNotSeen.x) / 2.0);
		_screen.y = ceil(((float)_lastSeen.y + _lastNotSeen.y) / 2.0);
	}
	else {
		_screen.x = floor(((float)_lastSeen.x + _lastNotSeen.x) / 2.0);
		_screen.y = floor(((float)_lastSeen.y + _lastNotSeen.y) / 2.0);
	}

	cout << "next pos: " << _screen.x << " " << _screen.y << "\n" << endl;

	// if we're looking for the up or down border, there should only be a 1 difference
	// in the y-coordinate of the two positions. For left/right in x-coordinate
	// The sum of these two differences should then be 1.
	if ((abs(_lastSeen.x - _lastNotSeen.x) + abs(_lastSeen.y - _lastNotSeen.y)) == 1) {
		// found point on the border
		switch (dir) {
		case 0:
			_firstUp = _lastSeen;
			break;
		case 1:
			_firstDown = _lastSeen;
			break;
		case 2:
			_firstLeft = _lastSeen;
			break;
		case 3:
			_firstRight = _lastSeen;
			break;
		}
		// go look for next first border position
		_binarySearchDirection++;
		_firstBinarySearchCall = true;
	}
}

//_____________________________________________________________________________
void CameraBorderDetector::borderWalker()
{
}

//_____________________________________________________________________________
void CameraBorderDetector::detectBorder() {
	// set up the initial positions for each direction
	pos up;
	up.x = _initPos.x;
	up.y = 0;

	pos down;
	down.x = _initPos.x;
	down.y = _screenHeight;

	pos left;
	left.x = 0;
	left.y = _initPos.y;

	pos right;
	right.x = _screenWidth;
	right.y = _initPos.y;

	// first find some position on each border
	// call has to be different for first time entering function
	if (_binarySearchDirection < 4) {
		if (_firstBinarySearchCall == true) {
			_firstBinarySearchCall = false;
			// call with init = true, initial position gets set
			switch (_binarySearchDirection) {
			case 0:
				binarySearch(_binarySearchDirection, up, true);
				break;
			case 1:
				binarySearch(_binarySearchDirection, down, true);
				break;
			case 2:
				binarySearch(_binarySearchDirection, left, true);
				break;
			case 3:
				binarySearch(_binarySearchDirection, right, true);
				break;
			}
		}
		else {
			// call with init = false, position is actually irrelevant here
			binarySearch(_binarySearchDirection, _initPos, false);
		}
	}
	else {
		// we know a position on all four borders
		// search for all the positions

		// search lower left corner

		// serach lower right corner

		// search upper left corner

		// search lower right corner

		// when all are found, set some bool to true

	}

	// if some bool is true, check whether the border is complete
	// start at one border position and check whether one of its nine
	// neighbours is another border pixel, if so take that as next position
	// if border is continuous - done
	// else - don't know yet?
}*/