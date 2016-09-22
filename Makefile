CXX=g++
CFLAGS=-Wall -std=c++11
SOURCE_DIR=src
SOURCE=main.cpp $(SOURCE_DIR)/*.cpp
LIBS=-lIrrlicht -lfreetype
INCLUDE=-Iinclude/ -Ideps/include -Ideps/include/irrlicht -Ideps/include/freetype2 -I/usr/include/irrlicht -I/usr/include/freetype2
EXECUTABLE=IrrlichtTest4

default: release

all: debug release

debug:
	mkdir bin/Debug -p
	$(CXX) $(CFLAGS) -g $(LIBS) $(INCLUDE) $(SOURCE) -o bin/Debug/$(EXECUTABLE)

release:
	mkdir bin/Release -p
	$(CXX) $(CFLAGS) $(LIBS) $(INCLUDE) $(SOURCE) -o bin/Release/$(EXECUTABLE)

clean:
	rm -rf bin/
	rm -rf obj/
