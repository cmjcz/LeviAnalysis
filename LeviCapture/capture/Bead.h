/*
	Bead.h

	The bead is a data structure who is representing a physical bead.

	@author Baptiste Senac
 */

#pragma once

#ifndef BEAD_H
#define BEAD_H

#include "opencv2/core/core.hpp"

class Bead {
public:
	Bead(cv::Point2f const center, float widthRatio = 1, float heightRatio = 1);
	/*
	Get the center of the bead, in px or in milimeters.
	*/
	cv::Point2f getCenterInPx() const;
	cv::Point2f getCenterInMm() const;
private:
	cv::Point2f const _center;
	float _widthRatio, _heightRatio;
};

#endif
