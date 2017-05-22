#include "ofMain.h"
#include "ofApp.h"
#include "detectingObjects.h"

//========================================================================
int main( ){



  ofSetupOpenGL(1024 , 768, OF_FULLSCREEN);


  //ofSetupOpenGL(1920, 1080, OF_WINDOW);// <-------- setup the GL context

  int horizon = ofGetWindowWidth();
  int vert = ofGetWindowHeight();

  cout << "width " << horizon << " height " << vert <<"\n";


  //ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	//ofRunApp(new ofApp());

  ofRunApp(new detectingObjects());

}
