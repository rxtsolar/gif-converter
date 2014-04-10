LIBS = -lopencv_core -lopencv_highgui -lopencv_imgproc -lgif
CFLAGS = -g -Wall
SRC = main.cpp grfmt_gif.cpp
CC = g++

all:
	$(CC) $(SRC) $(CFLAGS) $(LIBS)
