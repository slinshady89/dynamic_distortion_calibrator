# dynamic_distortion_calibrator
Automatic distortion calibration of any to the connected camera

this project was created with the project generator of openframeworks v0.9.8 . to compile the code open the project generator of openframeworks and import the project into the generator. After updating that you should be able to compile the code

## Functionality that is available right now:
The project currently consists of:
PixelSizeDetector
CameraAreaDetector(which should be replaced by CameraBorderDetector)
DynamicDistortionCalibrator

## Works in progress:
	PixelSizeDetector
Detect the minimum size of the square which the camera can recorgnize.-----DONE
	CameraBorderDetector
Detect at least one borderpiont of the camera frame using binary research;
Travel around the border from the beginning borderpoint.
	DynamicDistortionCalibrator
Create harps on the screen;
Save and load the data of raw distortion and maybe the model parameters; 
Recognize and regulate possible distortion.

## Next step:
•	Globally set a threshold for the white threshold and the camera device ID
•	Find the border of the camera frame
•	Calculate the raw distortion by subtracting the offsets from the screen positions stored at the image positions
•	Implement functions for loading and saving the raw distortion and maybe the model parameters
•	Fit a mathematical function of a given order to the raw distortion data

