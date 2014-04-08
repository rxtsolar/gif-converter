all:
	g++ main.cpp grfmt_gif.cpp -g -Wall -lopencv_core -lopencv_highgui -lgif
