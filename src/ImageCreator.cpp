#include "ImageCreator.h"



void ImageCreator::setup() {
  // set framerate
  ofSetFrameRate(20);
  // check for external web cam and use that if possible
  if (_cam.listDevices().size() > 1) {
    _cam.setDeviceID(1);
  }
  else {
    _cam.setDeviceID(0);
  }
  
  cout << "found camera\n";

  // setup camera
  _cam.setup(320, 240);
  cout << "setup camera\n";
  // get screen dimensions
  _screenHeight = ofGetWindowHeight();
  _screenWidth = ofGetWindowWidth();
  cout << "found screen dimensions\n";
  // get image dimensions
  _img = _cam.getPixels();
  _imgPixels = _img.getPixels();
  _imageHeight = _img.getHeight();
  _imageWidth = _img.getWidth();
  ofSetBackgroundAuto(false);
  ofBackground(ofColor::black);
  ofSetColor(ofColor::white);

  cout << "setup color stuffs\n beginning ground truth initialization\n";

  // initialize ground truths (same size)
  _vertical->groundTruth = new pos*[_screenWidth];
  _horizontal->groundTruth = new pos*[_screenWidth];

  for (int i = 0; i < _screenWidth; i++) {
	  _vertical->groundTruth[i] = new pos[_screenHeight];
	  _horizontal->groundTruth[i] = new pos[_screenHeight];
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
  }
}



void ImageCreator::draw() {
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
    // actually draws the pixel that should be detected for detection the borders of the camera frame
    ofFill();
    ofDrawRectangle(_screen.x, _screen.y, _pixelSize, _pixelSize);

    // next call capturing state
    if (_drawCount == 5) {  // minimum 4 for Nils' computer check all if you need higher timer!
      _drawCount = 0;
      _visDrawn = false;
      _state = 1;
    }

  }
  else if (_state == 1) {
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
    _state = 0;
  }
  else {
    // something went wrong
    cout << "Something went wrong, unreachable state reached. Quitting now.\n";
    ofGetWindowPtr()->setWindowShouldClose();
  }
}



void ImageCreator::setImageReturnVariables(calibrationImage *&vertical, calibrationImage *&horizontal, cameraBorder border, int pixelSize)
{
  _pixelSize = pixelSize;
  _border = border;

  _vertical = vertical;
  _horizontal = horizontal;
}


void ImageCreator::debugArea() {

  int centerX = _imageWidth / 2;
  int centerY = _imageHeight / 2;
  int cornerLeft = _imageWidth / 4;
  int cornerRight = _imageWidth * 3 / 4;
  int cornerTop = _imageHeight / 4;
  int cornerBot = _imageHeight * 3 / 4;

  for (int y = cornerTop; y < cornerBot; y++) {
    for (int x = cornerLeft; x < cornerRight; x++) {
      if (x == cornerLeft || x == cornerRight || y == cornerBot || y == cornerTop )
      _border._borderArray[x][y].x = x;
      _border._borderArray[x][y].y = y;
      // draws arbitrary borders white
      //_border._borderArray[x][y].b = 255;
    }
  }


}

void ImageCreator::findLines() {
  // find horizontal lines
  for (int y = 0; y < _imageHeight; y++) {
    int lowerX = 0;
    int count = 0;
    line actualLine;
    for (int x = 0; x < _imageWidth; x++) {
      if (_border._borderArray[x][y].border) {
        actualLine.y1 = actualLine.y2 = y;
        if (lowerX == 0) {
          lowerX = x;
          count++;
        }
        if (lowerX < x) {
          actualLine.x1 = lowerX;
          actualLine.x2 = x;
          count++;
        }
        if ((count == 2) && (actualLine.x1 != actualLine.x2) && (actualLine.x1 != 0) && (actualLine.x2 != 0)) {
          actualLine.horizontal = true;
          _horizontals.push_back(actualLine);
          count = 0;
          lowerX = 0;
        }
      }
    }
  }
  // find vertical lines
  for (int x = 0; x < _imageWidth; x++) {
    int lowerY = 0;
    line actualLine;
    int count = 0;
    for (int y = 0; y < _imageHeight; y++) {
      if (_border._borderArray[x][y].border) {
        actualLine.x1 = actualLine.x2 = x;
        if (lowerY == 0) {
          lowerY = y;
          count++;
        }
        if (lowerY <y) {
          actualLine.y1 = lowerY;
          actualLine.y2 = y;
          count++;
        }
        if ((actualLine.y1 != actualLine.y2) && (actualLine.y1 != 0) && (actualLine.y2 != 0)) {
          actualLine.vertical = true;
          _verticals.push_back(actualLine);
          count = 0;
          lowerY = 0;
        }
      }
    }
  }
}


void ImageCreator::drawDebug()
{
  ofImage img;
  img = _diffPixels;// difference between _cam;
  img.draw(0, 0);
  _cam.draw(0, 240);
  ofImage background;
  background = _background;
  background.draw(0, 480);
}