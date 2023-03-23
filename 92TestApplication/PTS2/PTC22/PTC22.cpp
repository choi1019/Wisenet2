
#include <92TestApplication/PTS2/PTC22/PTC22.h>
#include <92TestApplication/PTS2/PTC22/Config.h>

#include <19PApplication/PMain/PMain.h>

#include <opencv2/opencv.hpp>

using namespace cv;

Mat src, dst, src_gray;

PTC22::PTC22(unsigned nClassId, const char* pcClassName)
    : TestCase(nClassId, pcClassName)
{
}
PTC22::~PTC22() {
}

void PTC22::Initialize() {
}
void PTC22::Finalize() {
}
void PTC22::Run() {

	VideoCapture cap(0);
	
	if (!cap.isOpened())
	{
		printf("Can't open the camera");
		return;
	}

	Mat img;

	while (1)
	{
		cap >> img;

		imshow("camera img", img);

		if (waitKey(1) == 27)
			break;
	}



	return;
}