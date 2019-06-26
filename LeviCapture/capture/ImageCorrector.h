/*
	ImageCorrector.h

	An HomographyLens is a Lens who is applying every transformation needed to detect 
	the bead on the frame, and mesure his position in mm.
*/

#pragma once
#ifndef IMAGE_CORRECTOR_H
#define IMAGE_CORRECTOR_H
#include "Frame.h"
#include "Lens.h"

namespace capture {
	class HomographyLens : public Lens{
	public:
		HomographyLens(Frame const * backgroundFrame, Frame const * mesureFrame, Lens* nextLens = nullptr);
		~HomographyLens();
	protected:
		virtual Frame* correctFrame(Frame const * frame) const;
	private:
		cv::Mat _backgroundFrame, _mesureFrame, _homography;

		void initializeCorrectedCornerVector(std::vector<cv::Point2d>& correctedCorners, std::vector<cv::Point2d>& backgroundCorners, double imageWidth, double imageHeight) const;
	};
}

#endif