#include "grfmt_gif.h"

using namespace gs;
using namespace cv;

const int GS_FPS = 10;
const int GS_MAX_LENGTH = 4;
const int GS_WAIT = 1000 / GS_FPS;
const int GS_MAX_FRAMES = GS_MAX_LENGTH * GS_FPS;

int main(int argc, char* argv[])
{
    GifEncoder encoder;
    VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;
    namedWindow("Test");
    encoder.setFps(GS_FPS);
    for (int i = 0; i < GS_MAX_FRAMES; i++) {
        Mat frame;
        cap >> frame;
        encoder.addImage(frame);
        encoder.setFps(GS_FPS);
        imshow("Test", frame);
        if (waitKey(GS_WAIT) == 27)
            break;
    }
    encoder.writeGif("output.gif");
    return 0;
}
