#include "CameraAreaDetector.h"

//_____________________________________________________________________________
void CameraAreaDetector::setup()
{
	// set framerate
	ofSetFrameRate(21);
	pxSeenOnce = false;

	// check for external web cam and use that if possible
	_cam.setVerbose(false);
	if (_cam.listDevices().size() > 1) {
		_cam.setDeviceID(1);
	}
	else {
		_cam.setDeviceID(0);
	}
	// setup camera
	_cam.setup(_resolutionWidth, _resolutionHeight);

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

	// initialize image
	_area->_distortionX = new int*[_imageWidth];
	_area->_distortionY = new int*[_imageWidth];

	for (int i = 0; i < _imageWidth; i++) {
		_area->_distortionX[i] = new int[_imageHeight];
		_area->_distortionY[i] = new int[_imageHeight];
	}

	for (int x = 0; x < _imageWidth; x++) {
		for (int y = 0; y < _imageHeight; y++) {
			_area->_distortionX[x][y] = -1;
			_area->_distortionY[x][y] = -1;
		}
	}

	_test.allocate(_screenWidth, _screenHeight, ofImageType::OF_IMAGE_COLOR);
	_test.setColor(ofColor::blue);

	_previousX = -1;
	_previousY = -1;
	_firstBorderReached = false;
	_secondBorderReached = false;

	_x = 0;
	_y = 0;
	_distanceFromScreenX = 0;
	_distanceFromScreenX = 0;
	
	_leastX = INT_MAX;
	_leastY = INT_MAX;
	_largestX = 0;
	_largestY = 0;
	//test: _area->_borderArray[0][0].x = 100;
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
		ofFill();
		if (_borderState == 0) {
			// draw the rectangles for the initialPoint detection
			ofDrawRectangle(_screen.x, _screen.y, _pixelSize, _pixelSize);
		}
		else if (_borderState == 1) {
			// drawing the rectangle that grows over the screen
			ofDrawRectangle(0, 0, _screenWidth - _distanceFromScreenX, _screenHeight - _distanceFromScreenY);
		}
		else {
			fillOutArea();
			ofxCvColorImage test;
			test = _test;
			test.draw(0, 0);
			ofGetWindowPtr()->setWindowShouldClose();
		}
		// debug; draws the camera frame as seen, 
		if (debug == true) {
			drawDebug();
		}

		

		/*cout << "_state 0, _borderstate " << _borderState << "\n";
		ofSetColor(ofColor::white);
		ofFill();
		if(_borderState == 0) {
			// actually draws the pixel that should be detected for detection the borders of the camera frame
			ofDrawRectangle(_screen.x, _screen.y, _pixelSize, _pixelSize);
		}
		else if (_borderState == 1) {
			makeInitialCross(true);
		}
		else if (_borderState == 2) {
			makeInitialCross(false);
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
		}*/

		// next call capturing state
		if (_drawCount == 5) {  // minimum 4 for Nils' computer check all if you need higher timer!
			_drawCount = 0;
			_visDrawn = false;
			_state = 1;
		}

	} else if(_state == 1) {
		cout << "_state 1, _borderstate " << _borderState << "\n";
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
		if (_borderState == 0) {
			determineBrightestPosition();
			findInitialPosition();
			if (_initPosFound == true) {
				_borderState = 1;
			}
		}
		else if(_borderState == 1) {

			// get the difference between the background and the current image
			ofxCvGrayscaleImage diff;
			diff.setFromPixels(_background);
			diff.absDiff(_img);
			_diffPixels = diff.getPixels();
			cv::Mat temp = diff.getCvImage();
			// use canny to detect the edge of the rectangle on the screen
			// if it's not there, canny will simply find no edges
			cv::Mat edges;
			cv::Canny(temp, edges, _cannyLower, _cannyUpper);

			// get ofImage out of the cv::Mat object
			_contour.setFromPixels((unsigned char*)IplImage(edges).imageData, edges.size().width, edges.size().height, OF_IMAGE_GRAYSCALE);
			_diffPixels = _contour.getPixels();

			// creating the vertical edges
			if (_firstBorderReached == false) {
				cout << "_x: " << _x << " from " << _rightBorder << " to " << _leftBorder << "\n";
				// remember the edge in the current image
				writeLineCorrespondences(true);
				// increase the distance from the screen
				_distanceFromScreenX += 5;
				_x = _screenWidth - _distanceFromScreenX;
				// if the edge has walked far enough away from the initial point
				// cease its walk and continue on with the horizontal edges
				if (_x <= _leftBorder) {
					_y = _lowerBorder;
					_distanceFromScreenY = _screenHeight - _lowerBorder;
					_distanceFromScreenX = 0;
					_firstBorderReached = true;
				}
			}
			else if (_secondBorderReached == false) {
				writeLineCorrespondences(false);
				_distanceFromScreenY += 5;
				_y = _screenHeight - _distanceFromScreenY;
				if (_y <= _upperBorder) {
					_y = 0;
					_secondBorderReached = true;
					_borderState = 2;
				}
			}
		}
		/*
		cout << "_state 2, _borderstate " << _borderState << "\n";
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
			if (_secondBorderReached == true) {
				// reset border reached bools and proceed to next state
				_firstBorderReached = false;
				_secondBorderReached = false;
				_borderState = 4;
			}
		}
		else if (_borderState == 4) {
			writeHorizontalLines();
			if (_secondBorderReached == true) {
				_borderState = 5;
			}
		}
		else if (_borderState == 5) {
			calculateBorders();
		}*/

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
	img.resize(320, 240);
	img.draw(0, 0);
	ofImage cam = _cam.getPixels();
	cam.resize(320, 240);
	cam.draw(0, 240);
	ofImage background;
	background = _background;
	background.resize(320, 240);
	background.draw(0, 480);

	ofNoFill();
	if (_maxBrightness > WHITE_THRESHOLD) {
		// remap values to resized image [0, resolution] -> [0, resized]
		int x = (int) ((float)_maxBrightnessX / _resolutionWidth) * 320;
		int y = (int) ((float)_maxBrightnessY / _resolutionHeight) * 240;

		ofSetColor(ofColor::red);
		ofDrawEllipse(x, y, 40, 40);
		ofSetColor(ofColor::white);
	}
}

//_____________________________________________________________________________
void CameraAreaDetector::determineBrightestPosition()
{
	// subtract the background
	//_diffPixels = commonFunctions::subtractBackground(_img.getPixels(), _background);
	
	ofxCvGrayscaleImage diff;
	diff.setFromPixels(_background);
	diff.absDiff(_img);
	_diffPixels = diff.getPixels();

	// and finally detect the position of the brightest pixel in the image
	pos bright = commonFunctions::detectGrayValue(_diffPixels);
	_maxBrightness = bright.b;
	_maxBrightnessX = bright.x;
	_maxBrightnessY = bright.y;
	// assumed as white
	if (bright.b > WHITE_THRESHOLD) {
		if (_initPosFound == false) {
			// memorize outermost visible pixel coordinates
			if (_screen.x < _leastX) {
				_leastX = _screen.x;
			} else if (_screen.x > _largestX) {
				_largestX = _screen.x;
			}
			if (_screen.y < _leastY) {
				_leastY = _screen.y;
			}
			else if (_screen.y > _largestY) {
				_largestY = _screen.y;
			}

			// sum over the visible pixels
			_cumulativeX += _screen.x;
			_cumulativeY += _screen.y;
			// count the amount of seen pixels
			_seenCount++;
		}
		_pixelSeen = true;
	}
	else {
		_pixelSeen = false;
	}
  
}

//_____________________________________________________________________________
void CameraAreaDetector::setSpacing(int spacing) {
	_spacing = spacing;
}

//_____________________________________________________________________________
int CameraAreaDetector::getSpacing() {
	return _spacing;
}

//_____________________________________________________________________________
void CameraAreaDetector::findInitialPosition() {
  // run the pixel along the x-axis
  _screen.x += _spacing;

  // if the right border of the screen has been reached
  // begin again at the left border, but a row below
  if (_screen.x > _screenWidth) {
    _screen.x = 0;
    _screen.y += _spacing;
  }

  // if the next row would be below the screen's lower border
  // cease and calculate the center point
  if (_screen.y > _screenHeight) {
	// average over the seen pixel positions
	_initPos.x = ceil((float)_cumulativeX / (float)_seenCount);
    _initPos.y = ceil((float)_cumulativeY / (float)_seenCount);

	// calculate drawing borders
	// borders are outward from the outermost seen pixels by spacing + margin
	// unless out of screen dimensions
	_leftBorder = _leastX - (_spacing + 5) > 0 ? _leastX - (_spacing + 5) : 0;
	_rightBorder = _largestX + (_spacing + 5) < _screenWidth? _largestX + (_spacing + 5) : _screenWidth;
	_upperBorder = _leastY - (_spacing + 5) > 0 ? _leastY - (_spacing + 5) : 0;
	_lowerBorder = _largestY + (_spacing + 5) < _screenHeight? _largestY + (_spacing + 5) : _screenHeight;

	std::cout << _leastX << ", " << _largestX << "\n";
	std::cout << _leftBorder << ", " << _rightBorder << "\n";

	// offset from left screen border for drawing purposes
	int tmp = _screenWidth - _rightBorder; 
	_distanceFromScreenX = tmp > 0 ? tmp : 0;
	_x = _rightBorder;

    _initPosFound = true;
  }
}

//_____________________________________________________________________________
void CameraAreaDetector::writeLineCorrespondences(bool vert) {
	// get iterable data from the _contour image
	unsigned char* pixelData = _contour.getPixels().getPixels();

	// go over image and remember where the edge came from
	for (size_t x = 0; x < _imageWidth; x++) {
		for (size_t y = 0; y < _imageHeight; y++) {
			if (pixelData[_contour.getPixels().getPixelIndex(x, y)] > WHITE_THRESHOLD) {
				if (vert == true) {
					_area->_distortionX[x][y] = _x;
				}
				else {
					_area->_distortionY[x][y] = _y;
				}
			}
		}
	}
}

//_____________________________________________________________________________
void CameraAreaDetector::makeInitialCross(bool vertical) {
	if (vertical) {
		ofDrawRectangle(_initPos.x, 0, _pixelSize, _screenHeight);
	}
	else {
		ofDrawRectangle(0, _initPos.y, _screenWidth, _pixelSize);
	}
}

//_____________________________________________________________________________
void CameraAreaDetector::writeInitialCross(bool vertical) {
	ofxCvGrayscaleImage diff;
	diff.setFromPixels(_background);
	diff.absDiff(_img);
	unsigned char* pixelData = diff.getPixels().getPixels();

	// initPos specifies the left or upper border of the drawn lines with
	// _pixelSize width. Thus the line should be detected at the middle of 
	// that line. It gets floored so that it works with a pixelSize of 1, i.e.:
	// floor(_initPos.x + 0.5) = _initPos.x
	int indexX = floor(_initPos.x + float(_pixelSize) / 2.0);
	int indexY = floor(_initPos.y + float(_pixelSize) / 2.0);

	for (int x = 0; x < _imageWidth; x++) {
		for (int y = 0; y > _imageHeight; y++) {
			if (pixelData[diff.getPixels().getPixelIndex(x, y)] > WHITE_THRESHOLD) {
				if (vertical) {
					// we walk from left to right through the image
					// the first time we encounter a white pixel, it's part
					// of the initial cross line and thus yields the offset to
					// split the image into halves
					_area->_offsetX[y] = x;
					// wherever in the image the first vertical line is seen
					// in the screen it's x position hasn't changed from the
					// initial value
					_vertical->_matchX[x][y] = indexX;
					_vertical->_image.at<uchar>(x, y) = 255;
				}
				else {
					_horizontal->_horizontalOffsets[x] = y;
					_horizontal->_matchY[x][y] = indexY;
					_horizontal->_image.at<uchar>(x, y) = 255;
					// if the vertical offset and the horizontal offset match
					// this point is the initial point and now we know for a
					// single point, both camera and screen coordinates
					if (_vertical->_verticalOffsets[y] == x) {
						_horizontal->_initialPosition.x = indexX;
						_horizontal->_initialPosition.y = indexY;
						_vertical->_initialPosition.x = indexX;
						_vertical->_initialPosition.y = indexY;
					}
				}
			}
			else {
				_vertical->_image.at<uchar>(x, y) = 0;
				_horizontal->_image.at<uchar>(x, y) = 0;
			}
		}
	}
}

//_____________________________________________________________________________
void CameraAreaDetector::makeHorizontalLines() {
	// we start by drawing the lines from the initial line downwards, always
	// leaving out a space of one line, thus always drawing from two _pixelSizes
	// further away, as we don't want to draw the former line again (first _pixelSize)
	// and also want to leave out one (second _pixelSize)
	if (_previousY == -1) {
		ofDrawRectangle(0, _initPos.y + 2 * _pixelSize, _screenWidth, _pixelSize);
		if (_drawCount == 5) {
			_previousY = _initPos.y + 2 * _pixelSize;
		}
	}
	else if (_firstBorderReached == false) {
		// draw downwards
		ofDrawRectangle(0, _previousY + 2 * _pixelSize, _screenWidth, _pixelSize);
		if (_drawCount == 5) {
			_previousY += 2 * _pixelSize;
		}
	}
	else {
		// draw upwards
		// this means we have just entered this case for the first time and have
		// to start walking upwards from the initial position
		if (_previousY > _initPos.y) {
			_previousY = _initPos.y;
		}
		ofDrawRectangle(0, _previousY - 2 * _pixelSize, _screenWidth, _pixelSize);
		if (_drawCount == 5) {
			_previousY -= 2 * _pixelSize;
		}
	}
}

//_____________________________________________________________________________
void CameraAreaDetector::writeHorizontalLines() {
	ofxCvGrayscaleImage diff;
	diff.setFromPixels(_background);
	diff.absDiff(_img);
	unsigned char* pixelData = diff.getPixels().getPixels();

	// in order to not draw lines over the entire screen, check whether the lower border
	// of the image has been reached, by checking whether the line is still visible
	// if it hasn't been reached so far, claim nothing is visible until it's seen
	// in the for loop below.
	if (_firstBorderReached == true && _secondBorderReached == false) {
		_secondBorderReached = true;
	}
	if (_firstBorderReached == false) {
		_firstBorderReached = true;
	}

	for (int x = 0; x < _imageWidth; x++) {
		for (int y = 0; y > _imageHeight; y++) {
			cout << "(" << x << ", " << y << ")\n";
			if (pixelData[diff.getPixels().getPixelIndex(x, y)] > WHITE_THRESHOLD) {
				_horizontal->_image.at<uchar>(x, y) = 255;
				// can be set to this, because _previousY has already been set to the upper border
				// of the current line, and it should be detected at the middle of the line.
				_horizontal->_matchX[x][y] = floor(_previousY + (float)_pixelSize / 2.0);

				// if the first border is true, but the second is false, then we're looking for the
				// first border
				if (_firstBorderReached == true && _secondBorderReached == false) {
					// if the line is yet visible, we haven't reached this border
					_firstBorderReached = false;
				}
				else if (_firstBorderReached == true && _secondBorderReached == true) {
					// we're looking for the second border, or it wouldn't be true
					_secondBorderReached = false;
				}
			}
		}
	}
}

//_____________________________________________________________________________
void CameraAreaDetector::makeVerticalLines() {
	cout << "beginning making vertical lines\n";
	// we start by drawing the lines from the initial line to the left, always
	// leaving out a space of one line, thus always drawing from two _pixelSizes
	// further away, as we don't want to draw the former line again (first _pixelSize)
	// and also want to leave out one (second _pixelSize)
	if (_previousY == -1) {
		ofDrawRectangle(_initPos.x + 2 * _pixelSize, 0, _pixelSize, _screenHeight);
		if (_drawCount == 5) {
			_previousX = _initPos.x + 2 * _pixelSize;
		}
	}
	else if (_firstBorderReached == false) {
		// draw left
		ofDrawRectangle(_initPos.x + 2 * _pixelSize, 0, _pixelSize, _screenHeight);
		if (_drawCount == 5) {
			_previousX += 2 * _pixelSize;
		}
	}
	else {
		// draw right
		// this means we have just entered this case for the first time and have
		// to start walking to the right from the initial position
		if (_previousX > _initPos.x) {
			_previousX = _initPos.x;
		}
		ofDrawRectangle(_initPos.x - 2 * _pixelSize, 0, _pixelSize, _screenHeight);
		if (_drawCount == 5) {
			_previousX -= 2 * _pixelSize;
		}
	}
	cout << "finished making vertical lines \n";
}

//_____________________________________________________________________________
void CameraAreaDetector::writeVerticalLines() {
	cout << "beginning writeVerticalLines\n";
	ofxCvGrayscaleImage diff;
	diff.setFromPixels(_background);
	diff.absDiff(_img);
	unsigned char* pixelData = diff.getPixels().getPixels();

	cout << "initialised values\n";

	// in order to not draw lines over the entire screen, check whether the lower border
	// of the image has been reached, by checking whether the line is still visible
	// if it hasn't been reached so far, claim nothing is visible until it's seen
	// in the for loop below.
	if (_firstBorderReached == true && _secondBorderReached == false) {
		_secondBorderReached = true;
	}
	if (_firstBorderReached == false) {
		_firstBorderReached = true;
	}

	cout << "set booleans\n";

	for (int x = 0; x < _imageWidth; x++) {
		for (int y = 0; y > _imageHeight; y++) {
			cout << "(" << x << ", " << y << ")\n";
			if (pixelData[diff.getPixels().getPixelIndex(x, y)] > WHITE_THRESHOLD) {
				_vertical->_image.at<uchar>(x, y) = 255;
				// can be set to this, because _previousY has already been set to the upper border
				// of the current line, and it should be detected at the middle of the line.
				_vertical->_matchY[x][y] = floor(_previousX + (float)_pixelSize / 2.0);

				// if the first border is true, but the second is false, then we're looking for the
				// first border
				if (_firstBorderReached == true && _secondBorderReached == false) {
					// if the line is yet visible, we haven't reached this border
					_firstBorderReached = false;
				}
				else if (_firstBorderReached == true && _secondBorderReached == true) {
					// we're looking for the second border, or it wouldn't be true
					_secondBorderReached = false;
				}
			}
		}
	}
	cout << "finished writeVerticalLines()";
}

//_____________________________________________________________________________
void CameraAreaDetector::calculateBorders() {
	cv::Mat vert = _vertical->_image;
	cv::Mat hor = _horizontal->_image;
	pos init = _horizontal->_initialPosition;

	
}

//_____________________________________________________________________________
void CameraAreaDetector::fillOutArea() {
	int** posX;
	int** posY;

	// get the distortions
	posX = _area->_distortionX;
	posY = _area->_distortionY;
	
	for (int x = 0; x < _imageWidth; x++) {
		for (int y = 0; y < _imageHeight; y++) {
			// only fill in if both indices are known
			if (posX[x][y] != -1 && posY[x][y] != -1) {
				_test.setColor(posX[x][y], posY[x][y], ofColor::black);
			}
		}
	}
	std::cout << _leastX << ", " << _largestX << "\n";
	std::cout << _leftBorder << ", " << _rightBorder << "\n";
	std::cout << "lastX was: " << _x << "\n";
}

//_____________________________________________________________________________
void CameraAreaDetector::setResolutionWidth(int resolutionWidth) {
	_resolutionWidth = resolutionWidth;
}

//_____________________________________________________________________________
int CameraAreaDetector::getResolutionWidth() {
	return _resolutionWidth;
}

//_____________________________________________________________________________
void CameraAreaDetector::setResolutionHeight(int resolutionHeight) {
	_resolutionHeight = resolutionHeight;
}

//_____________________________________________________________________________
int CameraAreaDetector::getResolutionHeight() {
	return _resolutionHeight;
}

//_____________________________________________________________________________
void CameraAreaDetector::setCannyUpperThreshold(int upperThreshold) {
	_cannyUpper = upperThreshold;
}

//_____________________________________________________________________________
int CameraAreaDetector::getCannyUpperThreshold() {
	return _cannyUpper;
}

//_____________________________________________________________________________
void CameraAreaDetector::setCannyLowerThreshold(int lowerThreshold) {
	_cannyLower = lowerThreshold;
}

//_____________________________________________________________________________
int CameraAreaDetector::getCannyLowerThreshold() {
	return _cannyLower;
}