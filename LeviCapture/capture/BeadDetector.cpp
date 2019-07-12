#include "BeadDetector.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace capture;

BeadDetector::BeadDetector(float backWidthInMM, float backHeightInMM, float particleRadiusInMM, float circularity)
	: _backgroundWidth(backWidthInMM)
	, _backgroundHeight(backHeightInMM)
	, _circularity(circularity)
	, _particleRadius(particleRadiusInMM)
{}

Bead* BeadDetector::detectBead(cv::Mat const frame) const {
	cv::Mat working_mat = frame.clone();

	//1. Process corrected frame looking for blobs:
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::Canny(working_mat, working_mat, 0, 255, 3);//Reinforce edges
	cv::findContours(working_mat, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	int imageWidth = frame.cols, imageHeight = frame.rows;//Frame size
	//2. Check if any blob is our bead
	for (size_t i = 0; i < contours.size(); i++)
	{
		//2.1. Check some of its relevant parameters
		cv::Scalar color = cv::Scalar(255, 255, 255);
		double area = cv::contourArea(contours[i]);
		double perimeter = cv::arcLength(contours[i], true);
		cv::Point2f circleCentre;
		float circleRadiusInPix, circleRadiusInMM;
		cv::minEnclosingCircle(contours[i], circleCentre, circleRadiusInPix);
		float widthRatio = float(float(_backgroundWidth) / imageWidth);
		float heightRatio = float(float(_backgroundHeight) / imageHeight);
		circleRadiusInMM = circleRadiusInPix * widthRatio;

		//2.2. Circularity test
		double circularity = 4 * M_PI * area / (perimeter * perimeter);
		if (circleRadiusInMM > 0.5f && circularity > this->_circularity) {;
			return (new Bead(circleCentre, widthRatio, heightRatio));
		}
	}
	return nullptr;
}
