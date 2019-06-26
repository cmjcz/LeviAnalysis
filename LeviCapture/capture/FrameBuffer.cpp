#include "FrameBuffer.h"
#include "iostream"
using namespace capture;
void FrameBuffer::push(Frame* element, unsigned long no) {
	std::unique_lock<std::mutex> mlock(_mutex);
	_buffer.push(IndexedFrame{element, no});
	mlock.unlock();
	_cond.notify_one();
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