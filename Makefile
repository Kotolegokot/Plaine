CXX=g++
CFLAGS=-Wall -std=c++11
SOURCE=main.cpp src/*.cpp
LIB=-lfreetype -l:libPlaneRunnerIrrlicht.so.1.8 -lBulletDynamics -lBulletCollision -lLinearMath

SOURCE_INCLUDE=-Iinclude/ -Ideps/include/planerunner_irrlicht/ -I/usr/include/freetype2/ -I/usr/include/bullet
LIB_INCLUDE=-Ldeps/lib/ -Wl,-rpath,deps/lib/
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
	./start_debug.sh

release_run:
	./start_release.sh
