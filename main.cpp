#include "grfmt_gif.h"

using namespace gs;
using namespace cv;

const int GS_FPS = 2;
const int GS_MAX_LENGTH = 1;
const int GS_WAIT = 1000 / GS_FPS;
const int GS_MAX_FRAMES = GS_MAX_LENGTH * GS_FPS;

int main(int argc, char* argv[])
{
    GifEncoder encoder;
    VideoCapture cap(0);
    bool start = false;
    int i = 0;

    if (!cap.isOpened())
        return -1;

    namedWindow("Test");
    encoder.setFps(GS_FPS);

    while (i < GS_MAX_FRAMES) {
        char key;
        Mat frame;
        Mat after;
        cap >> frame;
        bilateralFilter(frame, after, 5, 1500, 150);
        if (start) {
            encoder.addImage(after);
            encoder.setFps(GS_FPS);
            i++;
        }
        imshow("Test", after);
        key = waitKey(GS_WAIT);
        if (key == 27)
            break;
        else if (key == 32)
            start = true;
    }
    encoder.writeGif("output.gif");
    return 0;
}
