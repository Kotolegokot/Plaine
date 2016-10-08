CXX=g++
CFLAGS=-Wall -std=c++11
SOURCE=main.cpp src/*.cpp
LIB=-lfreetype -lIrrlicht -lBulletDynamics -lBulletCollision -lLinearMath

SOURCE_INCLUDE=-Iinclude/ -Isrc/obstacles/ -isystem/usr/include/irrlicht/ -isystem/usr/include/freetype2/ -isystem/usr/include/bullet/
LIB_INCLUDE=-Ldeps/lib/
EXECUTABLE=PlaneRunner

default: release

all: debug release

debug:
	mkdir bin/Debug -p
	$(CXX) $(CFLAGS) -g $(LIB_INCLUDE) $(SOURCE_INCLUDE) $(SOURCE) $(LIB) -o bin/Debug/$(EXECUTABLE)

release:
	mkdir bin/Release -p
	$(CXX) $(CFLAGS) $(SOURCE_INCLUDE) $(LIB_INCLUDE) $(SOURCE) $(LIB) -o bin/Release/$(EXECUTABLE)

clean:
	rm -rf bin/
	rm -rf obj/

debug_run:
	bin/Debug/PlaneRunner

release_run:
	bin/Release/PlaneRunner
