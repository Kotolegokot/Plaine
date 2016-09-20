CC=g++
CFLAGS=-Wall -std=c++11
SOURCE_DIR=src
SOURCE=main.cpp $(SOURCE_DIR)/*.cpp
LIBS=-lIrrlicht
INCLUDE=-Iinclude/
EXECUTABLE=IrrlichtTest4

default: release

all: debug release

debug:
	mkdir bin/Debug -p
	$(CC) $(CFLAGS) -g $(LIBS) $(INCLUDE) $(SOURCE) -o bin/Debug/$(EXECUTABLE)

release:
	mkdir bin/Release -p
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDE) $(SOURCE) -o bin/Release/$(EXECUTABLE)

clean:
	rm -rf bin/
	rm -rf obj/
