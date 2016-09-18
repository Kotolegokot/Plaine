#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <irrlicht/irrlicht.h>

using namespace irr;

struct Context {
    Context(bool *pause, bool *quit, IrrlichtDevice *device) :
            pause(pause), quit(quit), device(device)
    {}

    bool *pause;
    bool *quit;
    IrrlichtDevice *device;
};

// GUI IDs
enum { ID_BUTTON_QUIT };

#endif // UTIL_H_INCLUDED
