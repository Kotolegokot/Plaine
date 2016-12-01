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
