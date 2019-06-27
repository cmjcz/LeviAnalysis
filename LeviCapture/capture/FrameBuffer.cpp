#include "FrameBuffer.h"
#include "iostream"
using namespace capture;
FrameBuffer::FrameBuffer(unsigned int nbMaxElemen) : _nbMaxElem(nbMaxElemen), _writtingFinished(false) {}

void FrameBuffer::push(Frame* element, unsigned long no) {
	bool canPush;
	do {
		canPush = _buffer.size() > _nbMaxElem;
		std::unique_lock<std::mutex> mlock(_mutex);
		if(canPush)
			_buffer.push(IndexedFrame{ element, no });
		mlock.unlock();
		_cond.notify_one();
	} while (!canPush);
}

IndexedFrame FrameBuffer::pop() {
	std::unique_lock<std::mutex> mlock(_mutex);
	while (_buffer.empty()) {
		_cond.wait(mlock);
	}
	IndexedFrame frame = _buffer.front();
	_buffer.pop();
	return frame;
}

size_t FrameBuffer::size() const{
	return _buffer.size();
}

bool FrameBuffer::isEmpty() const {
	return _buffer.empty();
}

bool FrameBuffer::isWrittingFinished() const {
	return _writtingFinished;
}

void FrameBuffer::setWrittingFinished(bool value) {
	_writtingFinished = value;
}