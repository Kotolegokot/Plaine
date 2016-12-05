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

#ifndef AUDIO_DEVICE_H
#define AUDIO_DEVICE_H

#include "audio/AudioChunk.h"
#include "audio/AudioStream.h"
#include "audio/AudioListener.h"
#include "audio/Priority.h"
#include "audio/OpenAL.h"

#include <string>
#include <memory>
#include <vector>
#include <algorithm>

using AudioHandle = std::shared_ptr<AudioSource>;

class AudioDevice {
private:
    std::unique_ptr<OpenAL> m_openAL;

    // Deletes unused audio memory, TODO
    static void bufferCleanup();

    static std::uint32_t grabAudioSource();
    static std::uint32_t leastImportantSource();
    static void wipeSource(std::uint32_t source);

public:
    AudioDevice();

    // Add channel support
    // store audio etc values by mapped channel names
    // set these values to audios put in that channel TODO

    static void registerAudio(const std::string &fileName, const std::string &name);

    static AudioHandle createStream(std::string fileName, PriorityType priority = Priority::Medium);
    static AudioHandle playStream(const std::string &fileName, PriorityType priority = Priority::Medium);

    static AudioHandle createSound(std::string fileName, PriorityType priority = Priority::Medium);
    static AudioHandle playSound(const std::string &fileName, PriorityType priority = Priority::Medium);

    void setPlaybackDevice(const std::string &playbackDevice);
    static void stopAllAudio();

    static std::size_t totalSoundCount();

    static std::vector<std::string> playbackDevices();
};

#endif // AUDIO_DEVICE_H
