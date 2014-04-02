#include <iostream>
#include <opencv2/opencv.hpp>
#include <gif_lib.h>

using namespace std;
using namespace cv;

class GifConverter {
public:
    GifConverter(const char* fileName)
    {
        image = imread(fileName, 1);
    }
    void showImage(void)
    {
        namedWindow("Test");
        imshow("Test", image);
        waitKey(0);
        destroyWindow("Test");
    }
    void writeGif(const char* fileName)
    {
        int ret;
        GifFileType* gif = EGifOpenFileName(fileName, false, 0);
        if (!gif) {
            cerr << "EGifOpenFileName failed: " << GifErrorString(gif->Error) << endl;
            exit(1);
        }

        GifByteType* output = new GifByteType[image.rows * image.cols];
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

        ret = EGifPutScreenDesc(gif, image.cols, image.rows, colorMap->BitsPerPixel, 0, colorMap);
        if (ret == GIF_ERROR) {
            cerr << "EGifPutScreenDesc failed: " << GifErrorString(gif->Error) << endl;
            exit(1);
        }

        GraphicsControlBlock controlBlock;
        GifByteType buf[4];

        controlBlock.TransparentColor = NO_TRANSPARENT_COLOR;
        controlBlock.DisposalMode = 1;
        controlBlock.UserInputFlag = false;
        controlBlock.DelayTime = 40;

        EGifGCBToExtension(&controlBlock, buf);
        ret = EGifPutExtension(gif, GRAPHICS_EXT_FUNC_CODE, 4, buf);
        if (ret == GIF_ERROR) {
            cerr << "EGifPutExtension failed: " << GifErrorString(gif->Error) << endl;
            exit(1);
        }

        ret = EGifPutImageDesc(gif, 0, 0, image.cols, image.rows, false, 0);
        if (ret == GIF_ERROR) {
            cerr << "EGifPutImageDesc failed: " << GifErrorString(gif->Error) << endl;
            exit(1);
        }

        EGifPutLine(gif, output, image.rows * image.cols);

        delete[] output;

        EGifCloseFile(gif);
    }
private:
    Mat image;

    void getColorMap(GifByteType* outputBuffer, ColorMapObject* colorMap)
    {
        int ret;
        int size = image.rows * image.cols;
        int colorMapSize = 256;
        GifByteType* b = new GifByteType[size];
        GifByteType* g = new GifByteType[size];
        GifByteType* r = new GifByteType[size];

        if (image.channels() != 3) {
            cerr << "image channels are " << image.channels() << endl;
            exit(1);
        }

        for (int i = 0; i < size; i++) {
            b[i] = image.data[3 * i];
            g[i] = image.data[3 * i + 1];
            r[i] = image.data[3 * i + 2];
        }

        ret = GifQuantizeBuffer(image.cols, image.rows, &colorMapSize, r, g, b, outputBuffer, colorMap->Colors);
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
    GifConverter gc("aaa_700.rnd.png");
    gc.showImage();
    gc.writeGif("output.gif");
    return 0;
}
