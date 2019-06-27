#include "ImageCorrector.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/highgui.hpp"

using namespace capture;

HomographyLens::HomographyLens(Frame const* backgroundFrame, Frame const* mesureFrame, Lens* nextLens) : Lens(nextLens) {
	_backgroundFrame = backgroundFrame->toOpenCvMat();
	
	cv::RNG rng(12345);
	cv::Mat canny_output;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	bool detected = false;
	std::vector<cv::Point2d> _backgroundCorners, _correctedCorners;

	//0. Take a copy for the background substraction.
	cv::Mat mat = mesureFrame->toOpenCvMat();
	mat.copyTo(_mesureFrame);
	cv::blur(mat, mat, cv::Size(3, 3));
	cv::Canny(mat, canny_output, 0, 255, 3);

	//1. Detect edges
	cv::findContours(canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
	for (size_t i = 0; !detected && i < contours.size(); i++) {
		cv::Scalar color = cv::Scalar(0, 0, 255);
		double area = cv::contourArea(contours[i]);
		//We assume the largest blob will be our black background square
		if (area > double(100) * 100) {
			cv::approxPolyDP(contours[i], _backgroundCorners, 4, true);
			if (_backgroundCorners.size() < 4)
				continue;
			//3. Found! Let's save relevant information:
			int imageWidth = mat.cols, imageHeight = mat.rows;//Frame size
			initializeCorrectedCornerVector(_correctedCorners, _backgroundCorners, imageWidth, imageHeight);
			_homography = cv::findHomography(_backgroundCorners, _correctedCorners);
			detected = true;
		}
	}
}

HomographyLens::~HomographyLens() {}

Frame* HomographyLens::correctFrame(Frame const* frame) const {
	cv::Mat sourceImage = frame->toOpenCvMat();
	delete frame;
	cv::subtract(sourceImage, _backgroundFrame, sourceImage);
	cv::add(sourceImage, _mesureFrame, sourceImage); // Because we are using only black and white, we don't have to worry about color addition
	cv::threshold(sourceImage, sourceImage, 90, 200, CV_THRESH_BINARY);
	unsigned int rows = sourceImage.cols / 2, cols = sourceImage.cols;
	cv::Size size(cols, rows);
	cv::Mat correctedImage;
	warpPerspective(sourceImage, correctedImage, _homography, sourceImage.size());
	cv::resize(correctedImage, correctedImage, size, 0, 0);

	return new MatFrame(correctedImage, resolution{unsigned int(correctedImage.cols), unsigned int(correctedImage.rows)});
}

void HomographyLens::initializeCorrectedCornerVector(std::vector<cv::Point2d>& correctedCorners, std::vector<cv::Point2d>& backgroundCorners, double imageWidth, double imageHeight) const {
	// Sorting points
	std::vector<int> x_axis, y_axis;
	for (int i = 0; i < 4; ++i) {
		x_axis.push_back(backgroundCorners[i].x);
		y_axis.push_back(backgroundCorners[i].y);
	}
	std::sort(x_axis.begin(), x_axis.end());
	std::sort(y_axis.begin(), y_axis.end());
	// Tagging points
	cv::Point2d LU, LD, RU, RD;
	for (unsigned int i = 0; i < 4; ++i) {
		cv::Point2d& p = backgroundCorners[i];
		if (p.x == x_axis[0] || p.x == x_axis[1]) { // Left
			if (p.y == y_axis[0] || p.y == y_axis[1]) LU = p; // UP
			else LD = p; // DOWN
		}
		else { // Right
			if (p.y == y_axis[0] || p.y == y_axis[1]) RU = p; // UP
			else RD = p; // DOWN
		}
	}

	// If the rectangle is vertical, you need to switch angles
	unsigned int width = RU.x - LU.x;
	unsigned int height = LD.y - LU.y;
	if (height > width) {
		cv::Point2d tmp = LU;
		LU = LD;
		LD = RD;
		RD = RU;
		RU = tmp;
	}

	// Setting correctedCorners
	for (unsigned int i = 0; i < 4; ++i) {
		cv::Point2d& p = backgroundCorners[i];
		if (p.x == RU.x && p.y == RU.y) correctedCorners.push_back(cv::Point2d(imageWidth - double(1), 0));
		else if (p.x == LU.x && p.y == LU.y) correctedCorners.push_back(cv::Point2d(0, 0));
		else if (p.x == LD.x && p.y == LD.y) correctedCorners.push_back(cv::Point2d(0, imageHeight - double(1)));
		else correctedCorners.push_back(cv::Point2d(imageWidth - double(1), imageHeight - double(1)));
	}
}