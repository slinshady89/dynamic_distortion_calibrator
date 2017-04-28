#include "ofApp.h"
#include <chrono>
#include <thread>

//--------------------------------------------------------------
void ofApp::setup(){
  // frame in which the camera signal will be shown
  //cam.setup(320, 240);

  int horizon = 0;
  int vert = 0;

}

//--------------------------------------------------------------
void ofApp::update(){
  // stetic update of the received Signal
  //cam.update();

}

//--------------------------------------------------------------
// Make a white plane on which one or more Pixel(s) are travelling 
void ofApp::draw() {

  // make white plane
  ofSetColor(255, 255, 255);
  ofDrawRectangle(0, 0, 1920, 1080);

  // get screen resolution
  int horizon = ofGetWindowWidth();
  int vert = ofGetWindowHeight();
  // output of screen resolution for testing
  string text = "width " + to_string(horizon) + " height " + to_string(vert);
  ofDrawBitmapStringHighlight(text,ofPoint(10,10,0.0),ofColor::white,ofColor::black );


  ofColor::red;
  ofDrawSphere(100,100, 1);
  Sleep(2500);
  ofColor::white;
  ofDrawSphere(100, 100, 1);
  Sleep(2200);
  ofColor::red;
  ofDrawSphere(200, 200, 1);
  Sleep(2500);
  ofColor::white;
  ofDrawSphere(200, 200, 1);
  Sleep(2500);


  /*
  
  for (int i = 0; i <= horizon; i++) 
  {
    for (int j = 0; j <= vert; j++)
    {
      ofColor::white;
      ofDrawSphere(i, j, 1);
      Sleep(1);
      ofColor::red;
      ofDrawSphere(i, j, 1);
    }
  }*/
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
