#include "ImageSaver.h"

#include <sstream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace capture;
using namespace std;

ImageSaver::ImageSaver(resolution res) :  _res(res) {}
ImageSaver::ImageSaver() : _res({0, 0}) {}
JpegSaver::JpegSaver(resolution res) : ImageSaver(res) {}

bool JpegSaver::saveImage(Frame& frame, string path) const {
	string ext = ".jpeg";
	cv::Mat opencvFrame = frame.toOpenCvMat();
	cv::imwrite(path + ext, opencvFrame);
	return true;
}