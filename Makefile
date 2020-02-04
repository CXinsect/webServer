CC = g++

CFLAGS = -lpthread -g -std=c++11

TARGET = Server

SrcFile = $(wildcard web.cc)

ObjFile = $(patsubst %.cpp,%.o,$(SrcFile))

%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET):$(ObjFile)
	$(CC) $(ObjFile) -o $(TARGET) $(CFLAGS) 

.PHONY:
	clean

clean:
	rm -rf Server
