#pragma once

#include "ofMain.h"

#include "wtypes.h"
#include <iostream>
using namespace std;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		

    // Object for camera signal
    //ofVideoGrabber cam;



    // Get the horizontal and vertical screen sizes in pixel
    void GetDesktopResolution(int& horizontal, int& vertical)
    {
      RECT desktop;
      // Get a handle to the desktop window
      const HWND hDesktop = GetDesktopWindow();
      // Get the size of screen to the variable desktop
      GetWindowRect(hDesktop, &desktop);
      // The top left corner will have coordinates (0,0)
      // and the bottom right corner will have coordinates
      // (horizontal, vertical)
      horizontal = desktop.right;
      vertical = desktop.bottom;
    }
};
