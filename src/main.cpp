#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

  int horizon = 0;
  int vert = 0;


  ofSetupOpenGL(1024, 768, OF_WINDOW);

  horizon = ofGetWindowWidth();
  vert = ofGetWindowHeight();

  cout << "width" << horizon << "height" << vert;


  //ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
