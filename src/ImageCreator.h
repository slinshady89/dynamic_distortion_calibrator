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

  // allows to set a pointer to a cv::Mat object
  // Is actually a workaround for the lack of return values in ofBaseApp
  void setImagePointer(cv::Mat *&image);
  void setPixelSize(int pixelSize);
  void setResolutionHeight(int resolutionHeight);
  void setResolutionWidth(int resolutionWidth);
  void setLinesToDraw(bool vert, bool hor);

private:
  // Object for camera signal
  ofVideoGrabber _cam;
  // pixelSize
  int _pixelSize;

  // keeps track of the state in draw() function
  int _state;
  // keeps track of how often the screen was drawn
  int _drawCount;
  // screenHeight
  int _screenHeight;
  // screenWidth
  int _screenWidth;
  // resolution
  int _resolutionHeight, _resolutionWidth;
  // image height
  int _imageHeight;
  // image width
  int _imageWidth;
  // color image taken from video grabber
  ofxCvColorImage _color;
  // grayscale image to be worked on
  ofxCvGrayscaleImage _img;
  // return image
  cv::Mat *_image;


  // if true draw horizontal lines, if both horizontal and vertical are true, draw both
  bool _drawHorizontal;
  // if true draw vertical, if both horizontal and vertical are true, draw both
  bool _drawVertical;
};