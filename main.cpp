#include "grfmt_gif.h"

using namespace gs;

int main(int argc, char* argv[])
{
    GifEncoder gc;
    gc.addImage("aaa_700.rnd.png");
    gc.addImage("aaa_705.rnd.png");
    gc.writeGif("output.gif");
    return 0;
}
