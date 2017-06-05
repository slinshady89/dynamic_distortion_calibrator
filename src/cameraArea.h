#pragma once
/*struct containing information about the area seen by a camera, used for dynamicDistortionCalibrator.
int** _imageX _imageY: 2D arrays of image size, contains the spots the screen drew on
int _offsetX _offsetY: offset from screen's borders in x and y direction respectively
int** _distortionX _distortionY: distortion in x or y direction respectively after subtraction of offset*/
struct cameraArea {
	/*two array with the size of the camera image. At position (x, y) is stored
	the coordinate at which the pixel that was detected at (x, y) got drawn on
	the screen*/
	int** _imageX;
	int** _imageY;

	// global offset, aka distance from middle of screen to middle of image
	int _offsetX;
	int _offsetY;

	// actual local distortion
	int** _distortionX;
	int** _distortionY;

	// sizes of image
	int _sizeImageX;
	int _sizeImageY;
};