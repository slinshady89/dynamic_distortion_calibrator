#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  // frame in which the camera signal will be shown
  //cam.setup(320, 240);
  ofBackground(ofColor::white);
  //set framerate (speed) of the travelling pixel
  ofSetFrameRate(60);
  x = 0;
  y = 0;

}

//--------------------------------------------------------------
void ofApp::update(){
  // stetic update of the received Signal
  //cam.update();

  // update the coordinates of the pixel with every new frame
  x += 1;
  if (ofGetWindowWidth() < x)
  {
    x = 0;
    y += 1;
  }
  if (ofGetWindowHeight() < y && ofGetWindowWidth() < x) {
    x = 0;
    y = 0;
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
  ofDrawRectangle(x, y,1, 1);

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
