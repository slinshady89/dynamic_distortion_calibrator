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
  debugArea();
  findStraightBorderConnections();
  drawHorizontals = true;
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
    if (drawHorizontals)
    {
      for (auto it : _horizontals)
      {
        ofDrawRectangle(it.x1, it.y1, it.x2 - it.x1, _pixelSize);
      }
      drawHorizontals = false;
    }
    else
    {
      for (auto it : _verticals)
      {
        ofDrawRectangle(it.x1, it.y1, _pixelSize, it.y2 - it.y1);
      }
    }
    
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

  _border._borderArray = new pos*[_screenWidth];
  for (int i = 0; i < _screenWidth; i++) {
	  _border._borderArray[i] = new pos[_screenHeight];
  }

  for (int y = cornerTop; y < cornerBot; y++) {
    for (int x = cornerLeft; x < cornerRight; x++) {
      if (x == cornerLeft || x == cornerRight || y == cornerBot || y == cornerTop ){
      _border._borderArray[x][y].x = x;
      _border._borderArray[x][y].y = y;
      // draws arbitrary borders white
      //_border._borderArray[x][y].b = 256;
      }
    }
  }


}

void ImageCreator::findStraightBorderConnections() {
  // find horizontal lines
  for (int y = 0; y < _imageHeight; y += 2*_pixelSize) {
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
  for (int x = 0; x < _imageWidth; x += 2 * _pixelSize) {
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
  saveGroundTruth(_horizontals);
  saveGroundTruth(_verticals);
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



void ImageCreator::saveGroundTruth(vector<line> &vectorOfLines) {
  // go threw the vectorOfLines and set borders of the 2 for-loops to
  // x1 -> x2 and y1 -> y2
  // so every pixel on the line should be drawn
  for (auto it : vectorOfLines){
    for (int y = it.y1; y <= it.y2; y++) {
      for (int x = it.x1; x <= it.x2; x++) {
        if (it.horizontal && !it.vertical)
        {
          _horizontal->groundTruth[x][y].x = x;
          _horizontal->groundTruth[x][y].y = y;
          _horizontal->groundTruth[x][y].b = 255;
        }
        // test isn't necessary but possible
        else if(it.vertical && !it.horizontal) {
          _vertical->groundTruth[x][y].x = x;
          _vertical->groundTruth[x][y].y = y;
          _vertical->groundTruth[x][y].b = 255;
        }
      }
    }
  }
}


void ImageCreator::countLines(cv::Mat &distImage) {
  // initial values
  cv::Mat vert = _vertical->_image;
  int* offsetX = new int[_imageHeight];
  offsetX = _vertical->_verticalOffsets;
  int* offsetY = new int[_imageWidth];
  offsetY = _horizontal->_horizontalOffsets;
  pos initialPos = _vertical->_initialPosition;

  // return values
  int* linesVert = new int[_imageHeight];
  int* linesHorizon = new int[_imageWidth];


  for (size_t y = initialPos.y; y < _imageHeight; y++){
    size_t initX = offsetX[y];
    int countLines = 0;
    int lastVal = -1;
    for (size_t x = initX; x < _imageWidth; x++) {
      size_t val = vert.at<uchar>(x, y);
      if (val == 255 && lastVal != val) {
        countLines++;
      }
      else if(val == 0 && lastVal != val) {
        countLines++;
      }
      lastVal = val;
    }
    linesVert[y] = countLines;
  }
  for (size_t y = initialPos.y; y < 0; y--) {
    size_t initX = offsetX[y];
    int countLines = 0;
    int lastVal = -1;
    for (size_t x = initX; x < 0; x--) {
      size_t val = vert.at<uchar>(x, y);
      if (val == 255 && lastVal != val) {
        countLines++;
      }
      else if (val == 0 && lastVal != val) {
        countLines++;
      }
      lastVal = val;
    }
    linesVert[y] = countLines;
  }



  for (size_t x = initialPos.x; x < _imageWidth; x++) {
    size_t initY = offsetY[x];
    int countLines = 0;
    int lastVal = -1;
    for (size_t y = initY; y < _imageHeight; y++) {
      size_t val = vert.at<uchar>(x, y);
      if (val == 255 && lastVal != val) {
        countLines++;
      }
      else if (val == 0 && lastVal != val) {
        countLines++;
      }
      lastVal = val;
    }
    linesHorizon[x] = countLines;
  }

  for (size_t x = initialPos.x; x < 0; x--) {
    size_t initY = offsetY[x];
    int countLines = 0;
    int lastVal = -1;
    for (size_t y = initY; y < 0; y--) {
      size_t val = vert.at<uchar>(x, y);
      if (val == 255 && lastVal != val) {
        countLines++;
      }
      else if (val == 0 && lastVal != val) {
        countLines++;
      }
      lastVal = val;
    }
    linesHorizon[x] = countLines;
  }



}




cv::Mat ImageCreator::interpolateImage(cv::Mat undistedImage) {
  cv::Mat interpolatedImage;
  // allocate the interpolatedImage that will be the return of the function with the size of the 
  // camera image
  interpolatedImage.zeros(undistedImage.size(), undistedImage.type());
  int rows = undistedImage.rows;
  int cols = undistedImage.cols;

  for (size_t x = 0; x < rows; x++) {
    for (size_t y = 0;y < cols; y++) {
      // if a seen value is at (x,y) position use that
      if (undistedImage.at<uchar>(x, y) != -1) {
        interpolatedImage.at<uchar>(x, y) = undistedImage.at<uchar>(x, y);
      }
      // else use bilinear interpolation of all accessible pixels around (x,y)
      else {
        size_t interpolate = 0;
        size_t count = 0;
        if (x + 1 < rows) {
          interpolate += (size_t)undistedImage.at<uchar>(x + 1, y);
          count++;
        }
        if (y + 1 < cols)
        {
          interpolate += (size_t)undistedImage.at<uchar>(x, y + 1);
          count++;
        }
        if (x - 1 > 0) {
          interpolate += (size_t)undistedImage.at<uchar>(x - 1, y);
          count++;
        }
        if (y - 1 < 0)
        {
          interpolate += (size_t)undistedImage.at<uchar>(x, y - 1);
          count++;
        }
        // if less than 2 pixel next to the not seen pixel are seen then it has to be at the border or not detected by the camera
        if (count > 1) {
          interpolatedImage.at<uchar>(x, y) = (uchar)floor(interpolate / count);
        }
      }
    }
  }
  return interpolatedImage;
}


cv::Mat ImageCreator::mappingImage(cv::Mat matchX, cv::Mat matchY, cv::Mat distortedImage) {
  cv::Mat mappedImage;
  size_t rows = distortedImage.rows;
  size_t cols = distortedImage.cols;
  size_t maxX, maxY, minX, minY;
  maxX = 0;
  maxY = 0;
  minX = rows;
  minY = cols;

  // find the extremal values of x & y  and save them for correct allocation of the mappedImage
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      size_t x = (size_t)matchX.at<uchar>(i, j);
      size_t y = (size_t)matchY.at<uchar>(i, j);
      if (x > maxX)
      {
        maxX = x;
      }
      if (y > maxY )
      {
        maxY = y;
      }
      if (x < minX)
      {
        minX = x;
      }
      if (y < minY)
      {
        minY = y;
      }
    }
  }
  // allocates the mapped undistorted matrix with zeros
  mappedImage.zeros(maxX-minX,maxY-minY, distortedImage.type());

  // checks the found matching matrices for the (x,y) distortion values and 
  // move the color values to the correct position 
  for (size_t x = 0; x < rows; x++) {
    for (size_t y = 0; y < cols; y++) {
      if (matchX.at<uchar>(x, y) != -1 && matchY.at<uchar>(x, y) != -1) {
        size_t distX = (size_t)matchX.at<uchar>(x, y) - minX + 1;
        size_t distY = (size_t)matchY.at<uchar>(x, y) - minY + 1;
        if (distX >= 0 && distY >= 0)
        {
          mappedImage.at<uchar>(distX, distY) = distortedImage.at<uchar>(x, y);
        }
      }
    }
  }
  return mappedImage;
}