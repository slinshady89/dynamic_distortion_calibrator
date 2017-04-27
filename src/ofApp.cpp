#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  // frame in which the camera signal will be shown
  //cam.setup(320, 240);

}

//--------------------------------------------------------------
void ofApp::update(){
  // stetic update of the received Signal
  //cam.update();

}

//--------------------------------------------------------------
// Draw a red circle that's center is at the position of the Cursor
void ofApp::draw() {
  ofSetColor(255, 0, 0);
  ofDrawSphere(500, 500, 1);

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
