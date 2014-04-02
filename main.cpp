#include <iostream>
#include <opencv2/opencv.hpp>
#include <gif_lib.h>

using namespace std;
using namespace cv;

class GifConverter {
public:
	GifConverter(const char* fileName)
	{
		image = imread(fileName, 0);
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
		GifPixelType* data;

		EGifSetGifVersion(gif, true);
		ret = EGifPutScreenDesc(gif, image.cols, image.rows, 0, 0, 0);
        if (ret == GIF_ERROR) {
            cerr << "EGifPutScreenDesc failed: " << GifErrorString(gif->Error) << endl;
            exit(1);
        }
		ret = EGifPutImageDesc(gif, 0, 0, image.cols, image.rows, false, 0);
        if (ret == GIF_ERROR) {
            cerr << "EGifPutImageDesc failed: " << GifErrorString(gif->Error) << endl;
            exit(1);
        }
		data = new GifPixelType[image.cols * image.rows];
        if (!data) {
            cerr << "not enough memory" << endl;
            exit(1);
        }
		memcpy(data, image.data, image.cols * image.rows * sizeof(GifPixelType));

		EGifPutLine(gif, data, image.rows * image.cols);
		EGifCloseFile(gif);
	}
private:
	Mat image;
};

int main(int argc, char* argv[])
{
    GifConverter gc("aaa_700.rnd.png");
    //gc.showImage();
    gc.writeGif("output.gif");
	return 0;
}
