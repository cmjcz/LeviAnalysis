#include "capture/Capture.h"

using namespace capture;
using namespace std;

int main(int argc, char* argv[]) {
	resolution res{1024, 576};
	FileVideoSource videoSource("D:\\test_pictures", ".jpeg", res);
	Frame* f = videoSource.getNextFrame();
	cv::imshow("test", f->toOpenCvMat());
	cv::waitKey();
	delete f;
	return 0;
}