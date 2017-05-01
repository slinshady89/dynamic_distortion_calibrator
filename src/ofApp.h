#pragma once

#include "ofMain.h"
#include "ofxGui.h"


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

    ofxPanel gui;
    ofParameter<int> x, y;

	private:
		int pixelSize;
		int windowHeight;
		int windowWidth;
		int spiralSize;
		int startX;
		int startY;
		int spiralDirection; // 0 -> up; 1 -> left; 2 -> down; 3 -> right

};
