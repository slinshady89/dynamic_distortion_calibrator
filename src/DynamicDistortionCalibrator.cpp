#include "DynamicDistortionCalibrator.h"

//_____________________________________________________________________________
DynamicDistortionCalibrator::DynamicDistortionCalibrator(int windowWidth , int windowHeight)
{
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
}

//_____________________________________________________________________________
DynamicDistortionCalibrator::~DynamicDistortionCalibrator()
{
}

//_____________________________________________________________________________
void DynamicDistortionCalibrator::findRawDistortion()
{
	// find pixelSize
	int pixelSize = findPixelSize();

	// find cameraArea
	_area = findCameraArea(pixelSize);

	// create images based on the found border
	createImages(pixelSize);

	// calculate the global offset from screen coordinates to image coordinates
	//calculateOffset();

	// correct for that offset, the values that remain in _area._distortionX/Y
	// are the local distortions
	// correctForOffset();
}

//_____________________________________________________________________________
void DynamicDistortionCalibrator::saveRawDistortion()
{
}

//_____________________________________________________________________________
void DynamicDistortionCalibrator::loadRawDistortion()
{
}

//_____________________________________________________________________________
int DynamicDistortionCalibrator::findPixelSize()
{
	// initialize pixelSize and pointer to pixelsize
	int pixelSize = 0;
	int *pixelSizePointer = &pixelSize;

	ofSetupOpenGL(_windowWidth, _windowHeight, OF_FULLSCREEN);// <-------- setup the GL context
	// create app for pixel size detection
	auto pixelSizeDetector = make_shared<PixelSizeDetector>();
	// set the app's pointer to the outside pixelSize variable
	pixelSizeDetector->setPixelSizePointer(pixelSizePointer);
	pixelSizeDetector->setResolutionHeight(_resolutionHeight);
	pixelSizeDetector->setResolutionWidth(_resolutionWidth);
	ofRunApp(pixelSizeDetector); // run app, closes once pixel found
	// print pixel size to screen
	std::cout << "DynDistCal: found pixelSize = " << pixelSize << "\n";

	return pixelSize;
}

//_____________________________________________________________________________
cameraArea DynamicDistortionCalibrator::findCameraArea(int pixelSize)
{
	// initialize the cameraArea and pointer to it
	cameraArea area;
	cameraArea *areaPointer = &area;

	ofSetupOpenGL(_windowWidth, _windowHeight, OF_FULLSCREEN);// <-------- setup the GL context
	
	// create the app for camera area detection
	auto cameraAreaDetector = make_shared<CameraAreaDetector>();
	// set the app's pointer
	cameraAreaDetector->setCameraAreaPointerAndPixelSize(areaPointer, pixelSize);
	// set the spacing, was calculated for our setup as optimal with a value of 34
	cameraAreaDetector->setSpacing(_spacing);
	cameraAreaDetector->setResolutionHeight(_resolutionHeight);
	cameraAreaDetector->setResolutionWidth(_resolutionWidth);
	cameraAreaDetector->setCannyLowerThreshold(_cannyLower);
	cameraAreaDetector->setCannyUpperThreshold(_cannyUpper);
	ofRunApp(cameraAreaDetector);
	std::cout << "found area \n";
	std::cout << area._distortionX[0][0] << "\n";

	return area;
}

//_____________________________________________________________________________
void DynamicDistortionCalibrator::createImages(int pixelSize)
{
	// initialize
	calibrationImage *verticalPointer = &_vertical;
	calibrationImage *horizontalPointer = &_horizontal;

	ofSetupOpenGL(_windowWidth, _windowHeight, OF_FULLSCREEN);// <-------- setup the GL context

	// create the app for the image creation
	auto imageCreator = make_shared<ImageCreator>();
	imageCreator->setImageReturnVariables(verticalPointer, horizontalPointer, pixelSize);
	ofRunApp(imageCreator);
	std::cout << "vgt[0][0].x = " << _vertical._groundTruth.at<uchar>(0, 0) << "\n";
}

//_____________________________________________________________________________
void DynamicDistortionCalibrator::setSpacing(int spacing) {
	_spacing = spacing;
}

//_____________________________________________________________________________
int DynamicDistortionCalibrator::getSpacing() {
	return _spacing;
}

//_____________________________________________________________________________
void DynamicDistortionCalibrator::setResolutionWidth(int resolutionWidth) {
	_resolutionWidth = resolutionWidth;
}

//_____________________________________________________________________________
int DynamicDistortionCalibrator::getResolutionWidth() {
	return _resolutionWidth;
}

//_____________________________________________________________________________
void DynamicDistortionCalibrator::setResolutionHeight(int resolutionHeight) {
	_resolutionHeight = resolutionHeight;
}

//_____________________________________________________________________________
int DynamicDistortionCalibrator::getResolutionHeight() {
	return _resolutionHeight;
}

//_____________________________________________________________________________
void DynamicDistortionCalibrator::setCannyUpperThreshold(int upperThreshold) {
	_cannyUpper = upperThreshold;
}

//_____________________________________________________________________________
int DynamicDistortionCalibrator::getCannyUpperThreshold() {
	return _cannyUpper;
}

//_____________________________________________________________________________
void DynamicDistortionCalibrator::setCannyLowerThreshold(int lowerThreshold) {
	_cannyLower = lowerThreshold;
}

//_____________________________________________________________________________
int DynamicDistortionCalibrator::getCannyLowerThreshold() {
	return _cannyLower;
}