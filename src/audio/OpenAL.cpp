/*

    The MIT License (MIT)

    Copyright (c) 2014 by Jakob Larsson

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
    IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 */

#include "audio/OpenAL.h"

OpenAL::OpenAL()
{
    changePlaybackDevice(nullptr); // use default system device

    Log::info("OpenAL startup information:");
    Log::info("\tvendor = ", alGetString(AL_VENDOR));
    Log::info("\tversion = ", alGetString(AL_VERSION));

    ALCint maxMono = 0, maxStereo = 0;
    alcGetIntegerv(openALPlaybackDevice, ALC_MONO_SOURCES, 1, &maxMono);
    alcGetIntegerv(openALPlaybackDevice, ALC_STEREO_SOURCES, 1, &maxStereo);

    Log::info("\tsupported source (mono) count = ", maxMono);
    Log::info("\tsupported source (stereo) count = ", maxStereo);
}

OpenAL::~OpenAL()
{
    alcMakeContextCurrent(nullptr);
    if (openALContext)
        alcDestroyContext(openALContext);
    if (openALPlaybackDevice)
        alcCloseDevice(openALPlaybackDevice);
}

void OpenAL::changePlaybackDevice(const ALchar *deviceName)
{
    alcMakeContextCurrent(nullptr);

    if (openALPlaybackDevice)
        alcCloseDevice(openALPlaybackDevice);
    if (openALContext)
        alcDestroyContext(openALContext);

    openALPlaybackDevice = alcOpenDevice(deviceName);

    if (!openALPlaybackDevice) {
        Log::warning("could not initialize OpenAL playback device (",
                     alcGetError(openALPlaybackDevice), ").");
        openALContext = nullptr;
        openALPlaybackDevice = nullptr;
    } else
        Log::info("OpenAL playback device opened (", alcGetString(openALPlaybackDevice,
                                                                  ALC_ALL_DEVICES_SPECIFIER), ")");

    openALContext = alcCreateContext(openALPlaybackDevice, nullptr);

    if (!openALContext || !alcMakeContextCurrent(openALContext)) {
        Log::warning("could not create and set OpenAL context.");
        openALPlaybackDevice = nullptr;
        openALContext = nullptr;
    }

}
