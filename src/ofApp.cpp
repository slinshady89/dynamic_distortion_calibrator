#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  // frame in which the camera signal will be shown
  cam.setup(320,240);
  ofBackground(ofColor::white);
  //set framerate (speed) of the travelling pixel
  ofSetFrameRate(240);
  windowHeight = ofGetWindowHeight();
  windowWidth = ofGetWindowWidth();
  startX = round(windowWidth / 2.0);
  startY = round(windowHeight / 2.0);
  x = startX;
  y = startY;
  pixelSize = 10;
  spiralSize = pixelSize;
  spiralDirection = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
  // stetic update of the received Signal
  cam.update();
  //cam.getPixels();


  // update the coordinates of the pixel with every new frame
  /*x += 1;
  if (ofGetWindowWidth() < x)
  {
    x = 0;
    y += 1;
  }
  if (ofGetWindowHeight() < y && ofGetWindowWidth() < x) {
    x = 0;
    y = 0;
  }*/

  // attempting to spiral pixel from middle point
  switch (spiralDirection) {
	case 0: std::cout << "in case 0\n";
		y--;
		if (y == startY - spiralSize) {
			spiralDirection = 1; // change direction to left
		};
		break;
	case 1: std::cout << "in case 1\n";
		x--;
		if (x == startX - spiralSize) {
			spiralDirection = 2; // change direction to left
			spiralSize += pixelSize;
		};
		break;
	case 2: std::cout << "in case 2\n";
		  y++;
		  if (y == startY + spiralSize) {
			  spiralDirection = 3; // change direction to left
		  };
		  break;
	case 3: std::cout << "in case 3\n";
		  x++;
		  if (x == startX + spiralSize) {
			  spiralDirection = 0; // change direction to left
			  spiralSize += pixelSize;
		  };
		  break;
	}

}

//--------------------------------------------------------------
// Make a white plane on which one or more Pixel(s) are travelling 
void ofApp::draw() {

  //// make white plane
  //ofSetColor(255, 255, 255);
  //ofDrawRectangle(0, 0, 1920, 1080);

  // get screen resolution
  //int horizon = ofGetWindowWidth();
  // vert = ofGetWindowHeight();

  // output of screen resolution for testing
  //string text = "width " + to_string(horizon) + " height " + to_string(vert);
  //ofDrawBitmapStringHighlight(text,ofPoint(10,10,0.0),ofColor::white,ofColor::black );

  // draw the travelling pixel
  ofSetColor(ofColor::red);
  ofDrawRectangle(x, y, pixelSize, pixelSize);

  string text = "breite" + to_string(cam.getWidth()) + "hoehe" + to_string(cam.getHeight());
  ofDrawBitmapStringHighlight(text,ofPoint(10,10,0.0),ofColor::white,ofColor::black );

  // finaly draw the camera frame 
  //cam.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){


}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
