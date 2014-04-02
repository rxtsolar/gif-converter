#include <iostream>
#include <opencv2/opencv.hpp>
#include <gif_lib.h>

using namespace std;
using namespace cv;

static GifColorType EGAPalette[] =      /* Default color map is EGA palette. */
{
    {   0,   0,   0 },   /* 0. Black */
    {   0,   0, 170 },   /* 1. Blue */
    {   0, 170,   0 },   /* 2. Green */
    {   0, 170, 170 },   /* 3. Cyan */
    { 170,   0,   0 },   /* 4. Red */
    { 170,   0, 170 },   /* 5. Magenta */
    { 170, 170,   0 },   /* 6. Brown */
    { 170, 170, 170 },   /* 7. LightGray */
    {  85,  85,  85 },   /* 8. DarkGray */
    {  85,  85, 255 },   /* 9. LightBlue */
    {  85, 255,  85 },   /* 10. LightGreen */
    {  85, 255, 255 },   /* 11. LightCyan */
    { 255,  85,  85 },   /* 12. LightRed */
    { 255,  85, 255 },   /* 13. LightMagenta */
    { 255, 255,  85 },   /* 14. Yellow */
    { 255, 255, 255 },   /* 15. White */
};

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
        ColorMapObject* colorMap = GifMakeMapObject(16, EGAPalette);
        if (!gif) {
            cerr << "EGifOpenFileName failed: " << GifErrorString(gif->Error) << endl;
            exit(1);
        }

		EGifSetGifVersion(gif, true);
		ret = EGifPutScreenDesc(gif, image.cols, image.rows, colorMap->BitsPerPixel, 0, colorMap);
        if (ret == GIF_ERROR) {
            cerr << "EGifPutScreenDesc failed: " << GifErrorString(gif->Error) << endl;
            exit(1);
        }
		ret = EGifPutImageDesc(gif, 0, 0, image.cols, image.rows, false, 0);
        if (ret == GIF_ERROR) {
            cerr << "EGifPutImageDesc failed: " << GifErrorString(gif->Error) << endl;
            exit(1);
        }

		EGifPutLine(gif, image.data, image.rows * image.cols);
		EGifCloseFile(gif);
	}
private:
	Mat image;
};

int main(int argc, char* argv[])
{
    GifConverter gc("aaa_700.rnd.png");
    gc.showImage();
    gc.writeGif("output.gif");
	return 0;
}
