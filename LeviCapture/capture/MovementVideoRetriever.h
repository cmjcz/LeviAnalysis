/*
	MovementVideoRetriever.h

	MovementVideoLoader is a VideoLoader who is using the movement analysis to tell if we want to load a frame or not.
	
	@author Baptiste Senac
*/

#pragma once

#ifndef MOVEMENT_VIDEO_RETRIEVER_H
#define MOVEMENT_VIDEO_RETRIEVER_H

#include "VideoLoader.h"
#include "MovementAnalysor.h"

namespace capture {
	class MovementVideoLoader : public VideoLoader {
	public:
		MovementVideoLoader(VideoSource& videoSource, MovementAnalysor* movementDetector, unsigned int delta_in_px);
		~MovementVideoLoader();
	protected:
		virtual unsigned long findFirstFrame(unsigned long startFrame, unsigned long endFrame) const;
		virtual unsigned long saveUntilLastFrame(FrameBuffer& pBuffer, unsigned long startFrame, unsigned long endFrame) const;
	private:
		MovementAnalysor* _movementDetector;
		unsigned int _delta_in_px;
	};
}

#endif
