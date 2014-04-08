#ifndef _GRFMT_GIF_H_
#define _GRFMT_GIF_H_

#include <opencv2/opencv.hpp>
#include <gif_lib.h>

namespace gs
{

class GifEncoder {
public:
    GifEncoder(void);
    void addImage(const char* fileName);
    void addImage(const cv::Mat& image);
    void writeGif(const char* fileName) const;
    int getFps(void) const;
    void setFps(int fps);
private:
    std::vector<cv::Mat> images;
    int width;
    int height;
    int fps;

    void getColorMap(GifByteType* outputBuffer, ColorMapObject* colorMap) const;
};

}

#endif /*_GRFMT_GIF_H_*/
