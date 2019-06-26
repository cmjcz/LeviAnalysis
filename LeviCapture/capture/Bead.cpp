#include "Bead.h"

Bead::Bead(cv::Point2f const center, float widthRatio, float heightRatio) : 
	_center(center), _widthRatio(widthRatio), _heightRatio(heightRatio) {}

cv::Point2f Bead::getCenterInPx() const {
	return _center;
}

cv::Point2f Bead::getCenterInMm() const {
	cv::Point2f circleCentreInMM(_center.x * _widthRatio, _center.y * _heightRatio);
	return circleCentreInMM;
}