/*
	VideoLoader.h

	A VideoLoader is an object to retrieve Frames from a video source,
	using what condition you want, and put them inside a FrameBuffer.
	The SimpleVideoRetriever given here just put all the Frames of the video source inside the buffer.

	@author Baptiste Senac
*/

#pragma once
#ifndef CAMERA_VIDEO_RETRIEVER_H
#define CAMERA_VIDEO_RETRIEVER_H

#include "VideoSource.h"
#include "FrameBuffer.h"
#include "opencv2/core/core.hpp"

namespace capture {
	class VideoLoader {
	public:
		VideoLoader(VideoSource& camera);
		void retrieveImages(FrameBuffer& pBuffer, unsigned long startFrame, unsigned long endFrame) const;
	protected:
		virtual unsigned long findFirstFrame(unsigned long startFrame, unsigned long endFrame) const = 0;
		virtual unsigned long saveUntilLastFrame(FrameBuffer& pBuffer, unsigned long startFrame, unsigned long endFrame) const = 0;
		VideoSource& _videoSource;
	};

	class SimpleVideoRetriever : public VideoLoader {
	public:
		SimpleVideoRetriever(VideoSource& camera);
	protected:
		virtual unsigned long findFirstFrame(unsigned long startFrame, unsigned long endFrame) const;
		virtual unsigned long saveUntilLastFrame(FrameBuffer& pBuffer, unsigned long startFrame, unsigned long endFrame) const;
	};
}

#endif