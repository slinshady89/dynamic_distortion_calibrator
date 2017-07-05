#pragma once

#include "Constants.h"

/*struct containing information about the area seen by a camera, used for dynamicDistortionCalibrator.
int _offsetX _offsetY: offset of the initial cross, for line counting method, not used for rectangle walking method
int** _distortionX _distortionY: distortion in x or y direction respectively*/
struct cameraArea {
	// global offset, aka distance from middle of screen to middle of image
	// not used for rectangle walking method
	int* _offsetX;
	int* _offsetY;

	// actual local distortion
	int** _distortionX;
	int** _distortionY;

	// sizes of image
	int _sizeImageX, _sizeImageY;
};