/*
	MovementAnalysor.h

	The movement analysor is a tool object, 
	who is implementing operation to analyze the movement of the bead
	between the first frame and the given frame.
*/

#pragma once

#ifndef MOVEMENT_DETECTOR_H
#define MOVEMENT_DETECTOR_H

#include "opencv2/core/core.hpp"
#include "BeadDetector.h"
#include "Frame.h"

#pragma region ERRORS_CODE

#define MOVED 0;
#define BEAD_NOT_FOUND 1;
#define NO_BASE_BEAD 2;
#define NOT_MOVED 3;

#pragma endregion

namespace capture {
	struct movement_2d_vector {
		double x, y;
	};

	class MovementAnalysor {
	public:
		MovementAnalysor(cv::Mat const base, BeadDetector& detector, float ignoreDeltaInPx = 10);
		MovementAnalysor(BeadDetector& detector, float ignoreDeltaInPx = 10);
		~MovementAnalysor();
		bool hasMoved(cv::Mat const newFrame) const;
		movement_2d_vector findRelativePositionInPx(cv::Mat const newFrame) const;
		movement_2d_vector findRelativePositionInPx(Bead const & bead) const;
		movement_2d_vector findRelativePositionInMm(cv::Mat const newFrame) const;
		movement_2d_vector findRelativePositionInMm(Bead const& bead) const;
		void setBase(cv::Mat const newFrame);
		bool isBaseSet();
	private:
		BeadDetector& _beadDetector;
		Bead* _pBaseBead;
		float _ignoreDeltaInPx;
	};
}

#endif
