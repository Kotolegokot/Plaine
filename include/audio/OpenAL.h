#ifndef OPENAL_H
#define OPENAL_H

#include <al.h>
#include <alc.h>
#include "Log.h"

class OpenAL {
    ALCdevice *openALPlaybackDevice = nullptr;
    ALCcontext *openALContext = nullptr;

public:
    OpenAL();
    ~OpenAL();

    void changePlaybackDevice(const ALchar *deviceName);
};

#endif // OPENAL_H
