
#include <92TestApplication/PTS2/PTC22/PTC22.h>
#include <92TestApplication/PTS2/PTC22/Config.h>

#include <19PApplication/PMain/PMain.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

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

    int rtn = 0;
    src = imread("data/test.jpg");
    cvtColor(src, src_gray, COLOR_RGB2GRAY);
    imshow("imgGray", src_gray);

    rtn = waitKey(0);
    if (rtn = 27) {
        return;
    }
    return;
}