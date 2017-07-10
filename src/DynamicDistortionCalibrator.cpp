#include "DynamicDistortionCalibrator.h"

//_____________________________________________________________________________
DynamicDistortionCalibrator::DynamicDistortionCalibrator(int windowWidth , int windowHeight)
{
	_pixelSize = 1;
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
}

//_____________________________________________________________________________
DynamicDistortionCalibrator::~DynamicDistortionCalibrator()
{
}

//_____________________________________________________________________________
void DynamicDistortionCalibrator::findRawDistortion(int** &matchX, int** &matchY)
{
	// find pixelSize
	_pixelSize = findPixelSize();

	// find cameraArea
	_area = findCameraArea();

	matchX = _area._distortionX;
	matchY = _area._distortionY;
}

//_____________________________________________________________________________
void DynamicDistortionCalibrator::saveRawDistortion(string path)
{
	ofstream file(path);

	if (file.is_open()) {
		// first print the sizes of the arrays
		file << _area._sizeImageX << "\n";
		file << _area._sizeImageY << "\n";

		// then print the arrays, first x-distortion
		for (int y = 0; y < _area._sizeImageY; y++) {
			for (int x = 0; x < _area._sizeImageX; x++) {
				file << _area._distortionX[x][y];
				if (y != _area._sizeImageX - 1) {
					file << ", ";
				}
			}
			file << ";\n";
		}
		// leave a line out
		std::cout << "saved x-map\n";
		file << "\n";
		// then y-distortion
		
		for (int y = 0; y < _area._sizeImageY; y++) {
			for (int x = 0; x < _area._sizeImageX; x++) {
				file << _area._distortionY[x][y];
				if (y != _area._sizeImageX - 1) {
					file << ", ";
				}
			}
			file << "\n";
		}
		std::cout << "saved y-map\n";
	}
	else {
		std::cout << "Saving the maps went wrong. File could not be openend.\n";
	}
}

//_____________________________________________________________________________
void DynamicDistortionCalibrator::loadRawDistortion(string path)
{
	ifstream file(path);
	string line;

	if (file.is_open()) {
		// first line is width of the array
		getline(file, line);
		int width = stoi(line);
		// second line is height of the array
		getline(file, line);
		int height = stoi(line);

		// allocate the arrays
		_area._sizeImageX = width;
		_area._sizeImageY = height;

		std::cout << "loaded sizes: " << width << " x " << height << "\n";

		_area._distortionX = new int*[width];
		_area._distortionY = new int*[width];

		for (int i = 0; i < width; i++) {
			_area._distortionX[i] = new int[height];
			_area._distortionY[i] = new int[height];
		}

		int count = 0;
		bool xComplete = false;
		// load x map
		// get first line
		getline(file, line);
		while(xComplete == false) {
			// work on that line
			_area._distortionX[count] = stringToIntArray(line);
			// get next line
			getline(file, line);
			++count;
			// if it's only a line break, cease
			if (line.compare("\n") == 0 || line.compare("") == 0) {
				xComplete = true;
			}
		}
		std::cout << "loaded x-map\n";
		// load y map
		count = 0;
		getline(file, line);
		while (!line.empty()) {
			// work on that line
			_area._distortionY[count] = stringToIntArray(line);
			// get next line
			getline(file, line);
			++count;
		}
		
	}
	else {
		std::cout << "Reading the maps went wrong. File could not be opened.\n";
	}
}

//_____________________________________________________________________________
int* DynamicDistortionCalibrator::stringToIntArray(string line) {
	string tmp;
	vector<int> vec;
	while (line.compare(";") != 0 && line.compare("") != 0) {
		tmp = line.substr(0, line.find_first_of(","));
		vec.push_back(stoi(tmp));
		line = line.substr(line.find_first_of(",") + 2, line.size());
	}

	int* arr = new int[vec.size()];
	for (size_t i = 0; i < vec.size(); i++) {
		arr[i] = vec.at(i);
	}

	return arr;
}


//_____________________________________________________________________________
void DynamicDistortionCalibrator::saveImageAsTxt(ofxCvGrayscaleImage pix, string fileName) {
	/*// for type ofPixels
	unsigned char* data;
	int width = pix.getWidth();
	int height = pix.getHeight();
	int noChannels = pix.getNumChannels();
	ofImageType type = pix.getImageType();
	
	std::cout << "noChannels: " << noChannels << endl;
	std::cout << "type: " << type << endl;

	ofstream file(fileName);

	if (file.is_open()) {
		// grab all the data from the channels
		ofPixels* channels = new ofPixels[noChannels];
		for (int i = 0; i < noChannels; i++) {
			channels[i] = pix.getChannel(i);
		}

		// first print the sizes of the arrays
		file << width << "\n";
		file << height << "\n";
		file << type << "\n";
		file << noChannels << "\n";
		file << pix.getPixelFormat() << "\n";

		ofPixels pixColor;
		pixColor.allocate(width, height, 1);

		// then print the arrays, first x-distortion
		for (size_t i = 0; i < noChannels; i++) {
			pixColor = pix.getChannel(i);
			for (size_t y = 0; y < height; y++) {
				for (size_t x = 0; x < width; x++) {
					file << pixColor.getColor(x, y);
					if (x != width - 1) {
						file << ", ";
					}
				}
				file << ";\n";
			}
			if (i != noChannels - 1) {
				std::cout << "went into next channel" << i << "\n";
				file << "next\n";
			}
		}

		delete[] channels;
	}
	else {
		std::cout << "Saving the maps went wrong. File could not be openend.\n";
	}*/

	cv::Mat mat;
	mat = pix.getCvImage();
	cv::Mat* matPointer = &mat;

	cv::FileStorage file;
	file.open(fileName, cv::FileStorage::WRITE);
	file.writeObj(fileName, matPointer);
	file.release();
}

//_____________________________________________________________________________
ofPixels DynamicDistortionCalibrator::loadImageAsTxt(string fileName) {
	/*ifstream file(fileName);
	string line;
	ofPixels pix;
	unsigned char* data;
	if (file.is_open()) {
		// first line is width of the array
		getline(file, line);
		int width = stoi(line);
		// second line is height of the array
		getline(file, line);
		int height = stoi(line);
		// third line is ofImageType
		getline(file, line);
		int typeNo = stoi(line);
		ofImageType type = static_cast<ofImageType>(stoi(line));

		std::cout << "type: " << type << "\n";
		
		// fourth line number of channels
		getline(file, line);
		int noChannels = stoi(line);

		// fifth line pixelFormat
		getline(file, line);
		ofPixelFormat format = static_cast<ofPixelFormat>(stoi(line));
		pix.pixelBitsFromPixelFormat(format);

		// allocate pix
		pix.allocate(width, height, type);
		pix.bytesFromPixelFormat(width, height, format);
		pix.setNumChannels(noChannels);

		data = new unsigned char[width * height];

		// go over all lines and grab color
		size_t channelCount = 0;
		size_t lineCount = 0;
		ofPixels pixHelper;
		pixHelper.allocate(width, height, type);
		pixHelper.bytesFromPixelFormat(width, height, format);
		int* tmp = new int[width];
		getline(file, line);
		while (!line.empty()) {
			// if one channel is completed and there's another
			if (line.compare("next") == 0) {
				// set this channel, increase count, reset lineCount
				//pix.setChannel(channelCount, pixHelper.getChannel(channelCount));
				pix.setFromExternalPixels(data, width, height, format);
				channelCount++;
				lineCount = 0;
				// get next line and take it from the top
				getline(file, line);
				continue;
			}
			// get numbers out of line
			tmp = stringToIntArray(line);
			// set this channels colors
			for (size_t x = 0; x < width; x++) {
				// y value is line count
				data[pix.getPixelIndex(x, lineCount)] = (unsigned char) tmp[x];
				if (x == 33) {
					cout << "data[33] = " << data[pix.getPixelIndex(x, lineCount)] << " and tmp[34] = " << (unsigned char) tmp[x] << endl;
				}
			}
			// read next line and increase line count
			getline(file, line);
			lineCount++;
		}
		// after everything was read in, set last channel
		pix.setFromExternalPixels(data, width, height, noChannels);
		std::cout << "im pix: " << (int) pix.getData()[pix.getPixelIndex(4, 0)] << endl;
	}
	else {
		std::cout << "Error: Could not open file.\n";
		return ofPixels();
	}
	return pix;*/

	ofFile file;
	file.open(fileName);

	ofImage img;
	img.loadImage(file);

	return img.getPixels();
}

//_____________________________________________________________________________
ofxCvGrayscaleImage DynamicDistortionCalibrator::createImage(bool vert, bool hor) {
	ofImage tmp;
	ofxCvGrayscaleImage returnImage;
	cv::Mat image;
	cv::Mat* imagePointer = &image;
	
	if (_pixelSize == -1) {
		std::cout << "ERROR: _pixelSize unknown. Returned image is unallocated!\n";
		return returnImage;
	}

	ofSetupOpenGL(_windowWidth, _windowHeight, OF_FULLSCREEN);// <-------- setup the GL context

	auto imageCreator = make_shared<ImageCreator>();

	imageCreator->setImagePointer(imagePointer);
	imageCreator->setPixelSize(_pixelSize);
	imageCreator->setResolutionHeight(_resolutionHeight);
	imageCreator->setResolutionWidth(_resolutionWidth);
	imageCreator->setLinesToDraw(vert, hor);

	ofRunApp(imageCreator);

	tmp.setFromPixels((unsigned char*)IplImage(image).imageData, image.size().width,
		image.size().height, OF_IMAGE_GRAYSCALE);
	returnImage = tmp;

	return returnImage;
}

//_____________________________________________________________________________
ofImage DynamicDistortionCalibrator::undistort(cv::Mat distortedImage, int** matchX, int** matchY) {
	cout << "starting undistortion\n";
	cv::Mat undistorted = mappingImage(distortedImage, matchX, matchY);
	cout << "mapping done\n";
	cv::Mat interpolated = interpolateImage(undistorted);
	cout << "interpolation done\n";
	// conversion to ofImage
	ofImage img;
	img.setFromPixels((unsigned char*)IplImage(interpolated).imageData, interpolated.size().width,
		interpolated.size().height, OF_IMAGE_GRAYSCALE);
	return img;
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

	return pixelSize;
}

//_____________________________________________________________________________
cameraArea DynamicDistortionCalibrator::findCameraArea()
{
	// initialize the cameraArea and pointer to it
	cameraArea area;
	cameraArea *areaPointer = &area;

	ofSetupOpenGL(_windowWidth, _windowHeight, OF_FULLSCREEN);// <-------- setup the GL context
	
	// create the app for camera area detection
	auto cameraAreaDetector = make_shared<CameraAreaDetector>();
	// set the app's pointer
	cameraAreaDetector->setCameraAreaPointerAndPixelSize(areaPointer, _pixelSize);
	// set the spacing, was calculated for our setup as optimal with a value of 70
	cameraAreaDetector->setSpacing(_spacing);
	cameraAreaDetector->setResolutionHeight(_resolutionHeight);
	cameraAreaDetector->setResolutionWidth(_resolutionWidth);
	cameraAreaDetector->setCannyLowerThreshold(_cannyLower);
	cameraAreaDetector->setCannyUpperThreshold(_cannyUpper);
	cameraAreaDetector->setJump(_jump);
	ofRunApp(cameraAreaDetector);

	return area;
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

//_____________________________________________________________________________
int** DynamicDistortionCalibrator::getMapX() {
	return _area._distortionX;
}

//_____________________________________________________________________________
int** DynamicDistortionCalibrator::getMapY() {
	return _area._distortionY;
}

//_____________________________________________________________________________
void DynamicDistortionCalibrator::setJump(int jump)
{
	_jump = jump;
}

//_____________________________________________________________________________
int DynamicDistortionCalibrator::getJump() {
	return _jump;
}

//_____________________________________________________________________________
cv::Mat DynamicDistortionCalibrator::interpolateImage(cv::Mat undistedImage) {
	cv::Mat interpolatedImage;
	// allocate the interpolatedImage that will be the return of the function with the size of the 
	// camera image
	interpolatedImage.zeros(undistedImage.size(), undistedImage.type());
	int rows = undistedImage.rows;
	int cols = undistedImage.cols;

	for (size_t x = 0; x < rows; x++) {
		for (size_t y = 0; y < cols; y++) {
			// if a seen value is at (x,y) position use that
			if (undistedImage.at<uchar>(x, y) != -1) {
				interpolatedImage.at<uchar>(x, y) = undistedImage.at<uchar>(x, y);
			}
			// else use bilinear interpolation of all accessible pixels around (x,y)
			else {
				size_t interpolate = 0;
				size_t count = 0;
				if (x + 1 < rows) {
					interpolate += (size_t)undistedImage.at<uchar>(x + 1, y);
					count++;
				}
				if (y + 1 < cols)
				{
					interpolate += (size_t)undistedImage.at<uchar>(x, y + 1);
					count++;
				}
				if (x - 1 > 0) {
					interpolate += (size_t)undistedImage.at<uchar>(x - 1, y);
					count++;
				}
				if (y - 1 < 0)
				{
					interpolate += (size_t)undistedImage.at<uchar>(x, y - 1);
					count++;
				}
				// if less than 2 pixel next to the not seen pixel are seen then it has to be at the border or not detected by the camera
				if (count > 1) {
					interpolatedImage.at<uchar>(x, y) = (uchar)floor(interpolate / count);
				}
			}
		}
	}
	return interpolatedImage;
}

//_____________________________________________________________________________
cv::Mat DynamicDistortionCalibrator::mappingImage(cv::Mat distortedImage, int** matchX, int** matchY) {
	//cv::Mat mappedImage;
	size_t rows = distortedImage.rows;
	size_t cols = distortedImage.cols;
	size_t maxX, maxY, minX, minY;
	maxX = 0;
	maxY = 0;
	minX = MAXSIZE_T;
	minY = MAXSIZE_T;

	// find the extremal values of x & y  and save them for correct allocation of the mappedImage
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			size_t x = (size_t)matchX[i][j];
			size_t y = (size_t)matchY[i][j];
			if (x != -1) {
				if (x > maxX)
				{
					maxX = x;
				}
				else if (x < minX)
				{
					minX = x;
				}
			}

			if (y != -1) {
				if (y > maxY)
				{
					maxY = y;
				}
				else if (y < minY)
				{
					minY = y;
				}
			}
		}
	}
	cout << "first loop to look for min and max done\n";
	// allocates the mapped undistorted matrix with zeros

	cv::Mat mappedImage(maxY - minY, maxX - minX, CV_32F, cv::Scalar::all(0));
	cout << "allocated image\n";
	// checks the found matching matrices for the (x,y) distortion values and 
	// move the color values to the correct position 
	for (size_t x = 0; x < rows; x++) {
		for (size_t y = 0; y < cols; y++) {
			if (matchX[x][y] != -1 && matchY[x][y] != -1) {
				std::cout << "matchX: " << matchX[x][y] - minY << ", matchY: " << matchY[x][y] - minY << " sizes: " << rows << "x" << cols << endl;
				size_t distX = (size_t)matchX[x][y] - minX;
				size_t distY = (size_t)matchY[x][y] - minY;
				if (distX >= 0 && distY >= 0)
				{
					std::cout << "error ";
					mappedImage.at<uchar>(distX, distY) = distortedImage.at<uchar>(x, y);
					cout << "here?\n";
				}
			}
		}
	}
	cout << "went over image\n";
	return mappedImage;
}