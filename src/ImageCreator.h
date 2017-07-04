#include "ofxOpenCv.h"
#include "ofMain.h"
#include "cameraArea.h"
#include "commonFunctions.h"
#include "Constants.h"
#include "Line.h"
#include "calibrationImage.h"

// creates an image with vertical, horizontal, or both lines, depending on the state of
// _drawHorizontal, and _drawVertical.
// On click the currently seen image will be handed back and also saved in:
// bin/data/distortedTestImage.jpg
// multiple images can be saved, simply by saving the image that's returned
class ImageCreator : public ofBaseApp {
public:
  void setup();
  void draw();
  void mousePressed(int x, int y, int button);

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
  // keeps track of how often the whole loop has been run
  int _loopCount;
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
  // interpolate missing allocations in a vertical or horizontal line in an image
  void interpolateLines(cv::Mat &distImage, bool vert);
};