#include <iostream>
#include <opencv2/opencv.hpp>
#include <gif_lib.h>

using namespace std;
using namespace cv;

class GifConverter {
public:
    GifConverter(void)
    {
        width = 0;
        height = 0;
    }

    void showImage(void)
    {
        namedWindow("Test");
        for (size_t i = 0; i < images.size(); i++) {
            imshow("Test", images[i]);
            waitKey(0);
        }
        destroyWindow("Test");
    }

    void addImage(const char* fileName)
    {
        Mat image = imread(fileName, 1);
        if (width == 0 && height == 0) {
            width = image.cols;
            height = image.rows;
        }
        if (width != image.cols || height != image.rows) {
            cerr << "Images size are different" << endl;
            exit(1);
        }
        images.push_back(image);
    }

    void writeGif(const char* fileName)
    {
        int ret;
        GifFileType* gif = EGifOpenFileName(fileName, false, 0);
        if (!gif) {
            cerr << "EGifOpenFileName failed: " << GifErrorString(gif->Error) << endl;
            exit(1);
        }

        GifByteType* output = new GifByteType[width * height * images.size()];
        if (!output) {
            cerr << "Allocating output buffer failed: Not enough memory" << endl;
            exit(0);
        }

        ColorMapObject* colorMap = GifMakeMapObject(256, 0);
        if (!colorMap) {
            cerr << "GifMakeMapObject failed" << endl;
            exit(1);
        }

        getColorMap(output, colorMap);

        EGifSetGifVersion(gif, true);

        ret = EGifPutScreenDesc(gif, width, height, colorMap->BitsPerPixel, 0, colorMap);
        if (ret == GIF_ERROR) {
            cerr << "EGifPutScreenDesc failed: " << GifErrorString(gif->Error) << endl;
            exit(1);
        }

        unsigned char param[3] = {1, 0, 0};
        GraphicsControlBlock controlBlock;
        EGifPutExtensionLeader(gif, APPLICATION_EXT_FUNC_CODE);
        EGifPutExtensionBlock(gif, 11, "NETSCAPE2.0");
        EGifPutExtensionBlock(gif, 3, param);
        EGifPutExtensionTrailer(gif);

        GifByteType buf[4];
        controlBlock.TransparentColor = NO_TRANSPARENT_COLOR;
        controlBlock.DisposalMode = 1;
        controlBlock.UserInputFlag = false;
        controlBlock.DelayTime = 40;

        for (size_t i = 0; i < images.size(); i++) {
            EGifGCBToExtension(&controlBlock, buf);

            ret = EGifPutExtension(gif, GRAPHICS_EXT_FUNC_CODE, 4, buf);
            if (ret == GIF_ERROR) {
                cerr << "EGifPutExtension failed: " << GifErrorString(gif->Error) << endl;
                exit(1);
            }

            ret = EGifPutImageDesc(gif, 0, 0, width, height, false, 0);
            if (ret == GIF_ERROR) {
                cerr << "EGifPutImageDesc failed: " << GifErrorString(gif->Error) << endl;
                exit(1);
            }

            EGifPutLine(gif, output + i * width * height, width * height);
        }

        delete[] output;

        EGifCloseFile(gif);
    }
private:
    vector<Mat> images;
    int width;
    int height;

    void getColorMap(GifByteType* outputBuffer, ColorMapObject* colorMap)
    {
        int ret;
        int size = width * height;
        int colorMapSize = 256;
        GifByteType* b = new GifByteType[size * images.size()];
        GifByteType* g = new GifByteType[size * images.size()];
        GifByteType* r = new GifByteType[size * images.size()];

        for (size_t i = 0; i < images.size(); i++) {
            for (int j = 0; j < size; j++) {
                b[i * size + j] = images[i].data[3 * j];
                g[i * size + j] = images[i].data[3 * j + 1];
                r[i * size + j] = images[i].data[3 * j + 2];
            }
        }

        ret = GifQuantizeBuffer(width, height * images.size(), &colorMapSize, r, g, b, outputBuffer, colorMap->Colors);
        if (ret == GIF_ERROR) {
            cerr << "GifQuantizeBuffer failed" << endl;
            exit(1);
        }

        delete[] b;
        delete[] g;
        delete[] r;
    }
};

int main(int argc, char* argv[])
{
    GifConverter gc;
    gc.addImage("aaa_700.rnd.png");
    gc.addImage("aaa_705.rnd.png");
    gc.showImage();
    gc.writeGif("output.gif");
    return 0;
}
