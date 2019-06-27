#include "FileVideoSource.h"
#include <filesystem>

#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
namespace fs = std::experimental::filesystem;

using namespace capture;

#define NB_FORMATS 13

std::string const supportedFormats[NB_FORMATS] =
{ "jpeg", "jpg", "jpe",
	"bmp", "dib", "png",
	"pbm", "pgm", "ppm",
	"sr", "ras", "tiff", "tif" };

static bool isEndingBy(std::string const& base, std::string const& end) {
	size_t baseLength = base.length(), endLength = end.length();
	if (baseLength >= endLength) return (0 == base.compare(baseLength - endLength, endLength, end));
	return false;
}

static bool isASupportedFormat(std::string const& path) {	
	unsigned int i = 0;
	bool isSupported = false;
	while (!isSupported && i < NB_FORMATS) {
		isSupported = isEndingBy(path, "." + supportedFormats[i]);
		++i;
	}
	return isSupported;
}

FileVideoSource::FileVideoSource(std::string folderName, resolution res, Lens* lens) : 
	VideoSource(res, lens), _folderName(folderName), _currentIndex(0) {
	std::smatch matches;
	for (const auto& file : fs::directory_iterator(folderName)) {
		std::string path = file.path().string();
		if (isASupportedFormat(path) != std::string::npos) {
			std::vector<std::string>::iterator it = std::lower_bound(_files.begin(), _files.end(), path);
			_files.insert(it, path);
		}
	}
}

FileVideoSource::~FileVideoSource() {}

Frame* FileVideoSource::getRawFrame(unsigned long frameNo) {
	if (frameNo >= _files.size()) return nullptr;
	std::string path = _files[frameNo];
	cv::Mat mat = cv::imread(path, cv::ImreadModes::IMREAD_GRAYSCALE);
	return new MatFrame(mat, _res);
}

Frame* FileVideoSource::getNextRawFrame() {
	unsigned long index = _currentIndex++;
	if (_currentIndex >= _files.size()) _currentIndex = 0;
	return getRawFrame(index);
}

unsigned int FileVideoSource::size() {
	return _files.size();
}