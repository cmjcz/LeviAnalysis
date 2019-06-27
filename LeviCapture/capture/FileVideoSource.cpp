#include "FileVideoSource.h"
#include <filesystem>

#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "iostream"
namespace fs = std::experimental::filesystem;

using namespace capture;

FileVideoSource::FileVideoSource(std::string folderName, std::string ext, resolution res, Lens* lens) : 
	VideoSource(res, lens), _folderName(folderName), _currentIndex(0), _ext(ext) {
	std::smatch matches;
	for (const auto& file : fs::directory_iterator(folderName)) {
		std::string path = file.path().string();
		if (path.find(ext) != std::string::npos) {
			std::vector<std::string>::iterator it = std::lower_bound(_files.begin(), _files.end(), path);
			_files.insert(it, path);
			std::cout << path << std::endl;
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