#include "capture/Capture.h"
#include <filesystem>
namespace fs = std::experimental::filesystem;

using namespace capture;
using namespace std;

int main(int argc, char* argv[]) {
	std::string path;
	bool isCorrectFolder = false;
	do {
		std::cout << "Please enter the path of the datas :" << std::endl;
		std::cin >> path;
		if (fs::exists(path)) isCorrectFolder = fs::is_directory(path);
	} while (!isCorrectFolder);
	cv::Mat backMat = cv::imread(path + "\\back.jpg", cv::ImreadModes::IMREAD_GRAYSCALE);
	cv::Mat mesureMat = cv::imread(path + "\\mesure.jpg", cv::ImreadModes::IMREAD_GRAYSCALE);
	resolution res = {backMat.cols, backMat.rows};
	Frame* backFrame = new MatFrame(backMat, res);
	Frame* mesureFrame = new MatFrame(mesureMat, res);
	HomographyLens* pLens = new HomographyLens(backFrame, mesureFrame);
	FileVideoSource videoSource(path + "\\out", res, pLens);
	BeadDetector detector(140, 70, 5);
	Frame* base = videoSource.getFrame(0);
	MovementAnalysor* analysor = new MovementAnalysor(base->toOpenCvMat(), detector);
	delete base;
	MovementVideoLoader* loader = new MovementVideoLoader(videoSource, analysor);
	VideoCsvSaver saver(loader, 1000);
	saver.saveVideo(0, videoSource.size());
	return 0;
}