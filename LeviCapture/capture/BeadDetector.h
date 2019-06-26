/*
	BeadDetector.h

	Detect one bead inside a cv::Mat

	@author Baptiste Senac, Diego
*/

#pragma once

#ifndef BEAD_DETECTOR_H
#define BEAD_DETECTOR_H

#include "Frame.h"
#include "opencv2/core/core.hpp"
#include "Bead.h"


namespace capture {
	class BeadDetector {
	private:
		//0. Background related parameters (bgnd substraction and homography correction)
		float _backgroundWidth, _backgroundHeight;
		std::vector<cv::Point2d> _backgroundCorners, _correctedCorners;
		//1. Detection features (particle size and circularity)
		float  _circularity, _particleRadius;

	public:
		BeadDetector(float backWidthInMM, float backHeightInMM, float particleRadiusInMM, float circularity = 0.8f);
		Bead* detectBead(cv::Mat const frame) const;
	};
}

#endif
