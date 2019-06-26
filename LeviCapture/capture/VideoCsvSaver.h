/**
	VideoCsvSaver.h

	VideoCsvSaver use an video to detect the movement of a bead, and save the informations inside a CSV file.

	@author Baptiste Senac
*/

#pragma once
#ifndef CAMERA_CSV_SAVER_H
#define CAMERA_CSV_SAVER_H

#include "VideoSaver.h"
#include "FrameBuffer.h"
#include "BeadDetector.h"
#include "MovementAnalysor.h"

namespace capture {
	class VideoCsvSaver : public VideoSaver {
	public:
		VideoCsvSaver(VideoLoader* videoRetriever, unsigned long framerate, unsigned int widthInMM = 140, unsigned int heightInMM = 70, unsigned int beadRadiusInMM = 5);
		virtual ~VideoCsvSaver();
	protected:
		virtual void beforeSave();
		virtual void saveOneFrame(std::string folderName, Frame* frame, unsigned long frameNo);
		virtual void afterSave(std::string folderName);
	private:
		BeadDetector _beadDetector;
		MovementAnalysor _analysor;
		unsigned long _framerate;
		std::stringstream _content;

		bool isFirstFrame = true;
		cv::Mat firstFrame;
		float previousTimeInMs = 0, previousVx = 0, previousVy = 0;
		unsigned long firstFrameNo = 0;
		movement_2d_vector mov = {0, 0}, previousMov = { 0, 0 };
	};
}

#endif
