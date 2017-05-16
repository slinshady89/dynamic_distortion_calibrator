# dynamic_distortion_calibrator
Automatic distortion calibration of any to the connected camera

this project was created with the project generator of openframeworks v0.9.8 . to compile the code open the project generator of openframeworks and import the project into the generator. After updating that you should be able to compile the code

## Functionality that is available right now:
we are able to move the pixel and detect some contours right now.

## Next step:
detect the smallest "pixel" which the camera can recognize.

## Works in progress:
1. create a smallest "pixel", until the camera can recognize it
2. detect the middlepoint of the camera
3. move the created "pixel" spiral formly until it had arrived the full frame of the camera
4. save and compare the positioninformation of the real position and the detected position
5. move the distorted pixelposition to the right position with an algorithm \ recognise and regulate possible distortion
