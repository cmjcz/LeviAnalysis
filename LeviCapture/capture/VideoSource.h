/*
	VideoSource.h

	A VideoSource is the object where you get the Frames wanted.
	It can use a Lens to change each Frames automatically.

	@author Baptiste Senac
*/

#pragma once

#ifndef VIDEO_SOURCE_H
#define VIDEO_SOURCE_H

#include "Frame.h"
#include "Lens.h"

namespace capture {
	class VideoSource {
	public:
		VideoSource(Lens* lens = nullptr);
		VideoSource(resolution res, Lens* lens = nullptr);
		virtual ~VideoSource();
		resolution getResolution();

		Frame* getFrame(unsigned long frameNo);
		Frame* getNextFrame() ;
		virtual unsigned int size() = 0;
	private:
		Lens* _lens;
	protected:
		resolution _res;

		virtual Frame* getRawFrame(unsigned long frameNo) = 0;
		virtual Frame* getNextRawFrame() = 0;
	};

	class FramesVideoSource : public VideoSource {
	public:
		FramesVideoSource(std::vector<Frame*> frames, resolution res, Lens* lens = nullptr);
		virtual ~FramesVideoSource();
		virtual unsigned int size();
	private:
		std::vector<Frame*> _frames;
		unsigned int _currentIndex;

	protected:
		virtual Frame* getRawFrame(unsigned long frameNo);
		virtual Frame* getNextRawFrame();
	};
}

#endif
