/**
	FrameBuffer.h

	A frame buffer is a thread safe queue for IndexedFrames.
	You have one or many producer, Objects who push Frames inside the buffer,
	and you have one or many consomer, Objects who pop IndexedFrames.
	The indexation is because when you parallelize the consomation, 
	you may need to know the order of the frames.

	IndexedFrame is a structure who is containing a Frame and the frame number.
	You must take care of the Frame*, because of the structure. 
	See the responsability of objects in the documentation.

	@author Baptiste Senac
*/

#pragma once

#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "opencv2/core/core.hpp"
#include "Frame.h"

namespace capture {

	struct IndexedFrame {
		Frame* frame;
		unsigned long index;
	};

	class FrameBuffer {
	public:
		FrameBuffer(unsigned int nbMaxElem = 1000);
		void push(Frame* element, unsigned long no);
		IndexedFrame pop();
		size_t size() const;
		bool isEmpty() const;
		bool isWrittingFinished() const;
		void setWrittingFinished(bool value);
	private:
		std::queue<IndexedFrame> _buffer;
		std::mutex _mutex;
		std::condition_variable _cond;
		bool _writtingFinished;
		unsigned int _nbMaxElem;
	};
}

#endif
