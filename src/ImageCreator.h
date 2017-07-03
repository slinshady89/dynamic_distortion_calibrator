#include "ofxOpenCv.h"
#include "ofMain.h"
#include "cameraArea.h"
#include "commonFunctions.h"
#include "Constants.h"
#include "Line.h"
#include "calibrationImage.h"

class ImageCreator : public ofBaseApp {
public:
  void setup();
  void draw();

  // allows to set a pointer to a cameraArea struct outside of this class in order
  // to keep the values found by this class and set the pixelSize
  // Is actually a workaround for the lack of return values in ofBaseApp
  void setImageReturnVariables(calibrationImage *&vertical, calibrationImage *&horizontal, int pixelSize);

private:

  // pointer to the cameraArea struct used for passing things to the outside
  cameraArea _area;
  // Object for camera signal
  ofVideoGrabber _cam;
  // pixelSize
  int _pixelSize;
  // replaces _screenX and _screenY so the max brightness that is detected for this drawn pixel could be saved to the coordinates
  pos _screen;

  // pointers to return variables
  calibrationImage *_vertical, *_horizontal;

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
  void findStraightBorderConnections();
  // debug function
  void drawDebug();
  // saves the ground truth straight lines into the screen image array
  void saveGroundTruth(vector<line> &vectorOfLines);
  // if true draw horizontal lines vertical if false 
  bool drawHorizontals;
  // calculate debug borders
  bool debugBorders;

  vector<line> _horizontals, _verticals;


  void countLines(cv::Mat &distImage);
};