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

#include "audio/AudioDevice.h"

namespace {
    std::unordered_map<std::string, std::string> m_audioFiles;

    using SoundData = std::pair<PriorityType, AudioHandle>;

    std::vector<SoundData> m_sounds;

    // Sounds can be played through channels which allows their volume
    // to be set collectively. TODO
    //std::unordered_map<std::string, float> m_audioChannels;
};

AudioDevice::AudioDevice() :
    m_openAL(std::make_unique<OpenAL>())
{}

void AudioDevice::registerAudio(const std::string &fileName, const std::string &name)
{
    auto it = m_audioFiles.find(name);
    if (it == m_audioFiles.end())
        m_audioFiles[name] = fileName;
}

AudioHandle AudioDevice::createStream(std::string fileName, PriorityType priority)
{
    auto it = m_audioFiles.find(fileName);

    if (it != m_audioFiles.end())
        fileName = it->second;

    auto filePtr = std::make_unique<AudioFile>();
    filePtr->tryOpen(fileName);

    std::uint32_t newSource = 0;
    if (filePtr->open())
        newSource = grabAudioSource();

    auto handle = std::make_shared<AudioStream>(std::move(filePtr), newSource);
    if (handle->valid())
        m_sounds.push_back(std::make_pair(priority, handle));

    return handle;
}

AudioHandle AudioDevice::playStream(const std::string &fileName, PriorityType priority)
{
    auto handle = createStream(fileName, priority);
    handle->play();

    return handle;
}

AudioHandle AudioDevice::createSound(std::string fileName, PriorityType priority)
{
    auto it = m_audioFiles.find(fileName);
    if (it != m_audioFiles.end())
        fileName = it->second;

    auto filePtr = std::make_unique<AudioFile>();
    filePtr->tryOpen(fileName);

    std::uint32_t newSource = 0;
    if (filePtr->open())
        newSource = grabAudioSource();

    //auto handle = std::make_shared<AudioChunk>(std::move(filePtr), newSource);
    auto handle = AudioHandle(new AudioChunk(std::move(filePtr), newSource));
    if (handle->valid())
        m_sounds.push_back(std::make_pair(priority, handle));

    return handle;
}

AudioHandle AudioDevice::playSound(const std::string &fileName, PriorityType priority)
{
    auto handle = createSound(fileName, priority);
    handle->play();

    return handle;
}

void AudioDevice::bufferCleanup()
{
    /*std::size_t buffersRemoved = 0;

    for (auto &audioFile : m_audioFiles) {
        if (audioFile.second.buffer.use_count() == 1) {
            if (alIsBuffer(*audioFile.second.buffer))
                alDeleteBuffers(1, audioFile.second.buffer.get());

            *audioFile.second.buffer = 0;
            ++buffersRemoved;
        }
    }*/
}

void AudioDevice::wipeSource(std::uint32_t source)
{
    alSourceRewind(source);
    alSourcei(source, AL_BUFFER, 0);

    alSourcei(source, AL_LOOPING, AL_FALSE);
    alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE);
    alSourcei(source, AL_GAIN, 1);
    alSourcef(source, AL_PITCH, 1.0f);
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSource3f(source, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
}

std::uint32_t AudioDevice::grabAudioSource()
{
    std::uint32_t newSource = 0;

    for (std::size_t i = 0; i < m_sounds.size(); i++) {
        auto &handle = m_sounds[i].second;

        if (!handle->playing() && handle.use_count() == 1) {
            newSource = handle->invalidate();
            wipeSource(newSource);
            m_sounds.erase(m_sounds.begin() + i);
            return newSource;
        }
    }

    alGetError();
    alGenSources(1, &newSource);

    ALenum sourceError = alGetError();

    if (sourceError != AL_NO_ERROR)
        newSource = leastImportantSource();

    wipeSource(newSource);
    return newSource;
}

std::uint32_t AudioDevice::leastImportantSource()
{
    auto eraseAndReturn = []() -> std::uint32_t
    {
        auto &handle = m_sounds[0].second;
            std::uint32_t newSource = handle->invalidate();

            m_sounds.erase(m_sounds.begin());
            return newSource;
    };

    std::sort(m_sounds.begin(), m_sounds.end(),
              [](const SoundData &lhs, const SoundData &rhs)
              {
                  return lhs.first < rhs.first;
              });

    PriorityType lowestPriority = m_sounds[0].first;
    unsigned int lowestPriorityCount = 0;
    for (auto &sound : m_sounds) {
        if (sound.first != lowestPriority)
            break;
        else
            ++lowestPriorityCount;
    }

    if (lowestPriorityCount == 0)
        return eraseAndReturn();

    std::sort(m_sounds.begin(), m_sounds.begin() + lowestPriorityCount,
              [](const SoundData &lhs, const SoundData &rhs) -> bool
              {
                  return lhs.second->volume() < rhs.second->volume();
              });

    float lowestVolume = m_sounds[0].second->volume();
    unsigned int lowestVolumeCount = 0;
    for (auto &sound : m_sounds) {
        if (sound.second->volume() != lowestVolume)
            break;
        else
            ++lowestVolumeCount;
    }

    if (lowestVolumeCount == 0)
        return eraseAndReturn();

    std::sort(m_sounds.begin(), m_sounds.begin() + lowestVolumeCount,
              [](const SoundData &lhs, const SoundData &rhs) -> bool
              {
                  return lhs.second->position().distance(AudioListener::position()) >
                         rhs.second->position().distance(AudioListener::position());
              });

    float furthestDistance = m_sounds[0].second->position().distance(AudioListener::position());
    unsigned int furthestDistanceCount = 0;
    for (auto &sound : m_sounds) {
        float curDistance = sound.second->position().distance(AudioListener::position());
        if (curDistance != furthestDistance)
            break;
        else
            ++furthestDistanceCount;
    }

    std::sort(m_sounds.begin(), m_sounds.begin() + furthestDistanceCount,
              [](const SoundData &lhs, const SoundData &rhs) -> bool
              {
                  return (lhs.second->duration() - lhs.second->offset()) <
                         (rhs.second->duration() - rhs.second->offset());
              });

    return eraseAndReturn();
}

void AudioDevice::setPlaybackDevice(const std::string &playbackDevice)
{
    m_openAL->changePlaybackDevice(playbackDevice.c_str());
}

void AudioDevice::stopAllAudio()
{
    for (auto &sound : m_sounds)
        sound.second->stop();

    Log::info("stopped all audio playback.");
}

std::size_t AudioDevice::totalSoundCount()
{
    return m_sounds.size();
}

std::vector<std::string> AudioDevice::playbackDevices()
{
    const ALCchar *rawVector = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
    std::vector<std::string> realVector;

    while (*rawVector) {
        std::string curDevice = std::string(rawVector);
        rawVector += curDevice.size() + 1;
        realVector.push_back(curDevice);
    }

    return realVector;
}
