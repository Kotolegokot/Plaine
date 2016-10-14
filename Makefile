CFLAGS=-Wall -std=c++14
SOURCE=main.cpp src/*.cpp src/obstacles/*.cpp
LIB=-lfreetype -lIrrlicht -lBulletDynamics -lBulletCollision -lLinearMath

SOURCE_INCLUDE=-Iinclude/ -isystem/usr/include/irrlicht/ -isystem/usr/include/freetype2/ -isystem/usr/include/bullet/
LIB_INCLUDE=-Ldeps/lib/
EXECUTABLE=PlaneRunner

default: release

all: debug release

debug:
	mkdir bin/Debug -p
	$(CXX) $(CFLAGS) -g $(LIB_INCLUDE) $(SOURCE_INCLUDE) $(SOURCE) $(LIB) -o bin/Debug/$(EXECUTABLE)

release:
	mkdir bin/Release -p
	$(CXX) $(CFLAGS) -O3 -DDEBUG_OUTPUT=false $(SOURCE_INCLUDE) $(LIB_INCLUDE) $(SOURCE) $(LIB) -o bin/Release/$(EXECUTABLE)

clean:
	rm -rf bin/
	rm -rf obj/

debug_run:
	echo ==== NEW START ==== >> logfile
	bin/Debug/PlaneRunner >> logfile

run: release_run
release_run:
	bin/Release/PlaneRunner
