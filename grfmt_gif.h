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
    void writeGif(const char* fileName);
private:
    std::vector<cv::Mat> images;
    int width;
    int height;

    void getColorMap(GifByteType* outputBuffer, ColorMapObject* colorMap);
};

}

#endif /*_GRFMT_GIF_H_*/
