#include "ofxOpenCv.h"
#include "ofMain.h"
#include "cameraBorder.h"
#include "commonFunctions.h"
#include "Line.h"

class ImageCreator : public ofBaseApp {
public:
  void setup();
  void draw();

  // allows to set a pointer to a cameraArea struct outside of this class in order
  // to keep the values found by this class and set the pixelSize
  // Is actually a workaround for the lack of return values in ofBaseApp
  void setCameraAreaPointerAndPixelSize(cameraBorder *&area, int pixelSize);

private:

  // pointer to the cameraArea struct used for passing things to the outside
  cameraBorder *_area;
  // Object for camera signal
  ofVideoGrabber _cam;
  // pixelSize
  int _pixelSize;
  // replaces _screenX and _screenY so the max brightness that is detected for this drawn pixel could be saved to the coordinates
  pos _screen;

  // keeps track of the state in draw() function
  int _state;
  // keeps track of how often the screen was drawn
  int _drawCount;
  // screenHeight
  int _screenHeight;
  // screenWidth
  int _screenWidth;
  // image height
  int _imageHeight;
  // image width
  int _imageWidth;
  // color image taken from video grabber
  ofxCvColorImage _color;
  // grayscale image to be worked on
  ofxCvGrayscaleImage _img;
  // pixels of image
  ofPixels _imgPixels;
  // background
  ofPixels _background;
  // difference between image and background
  ofPixels _diffPixels;
  // image drawn as background for visualization purposes
  ofPixels _vis;

  bool _visDrawn;
  // background set
  bool _backgroundSet;




  // draws a arbitrarily area that could be used as a possible camera frame
  void debugArea();

  // draws horizontal and vertical lines for distortion calibration
  void findLines();
  // debug function
  void drawDebug();




  vector<line> _horizontals, _verticals;
};