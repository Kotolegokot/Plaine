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

#ifndef AUDIO_SOURCE_H
#define AUDIO_SOURCE_H

#include <memory>
#include <chrono>
#include <al.h>
#include <alc.h>
#include "util/Vector3.h"
#include "audio/AudioFile.h"

using duration_t = std::chrono::duration<float>;

class AudioSource {
    friend class AudioDevice;

    bool m_isValidSource = true;

    std::uint32_t invalidate();
    duration_t m_duration;
    Vector3<float> m_position { 0, 0, 0 };
public:
    enum class AudioStatus {
        Playing,
        Paused,
        Stopped
    };

    virtual ~AudioSource();

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;

    void setPitch(float pitch);
    void setPosition(const Vector3<float> &position);
    void setVelocity(const Vector3<float> &velocity);
    void setDirection(const Vector3<float> &direction);
    void setRelativeToListener(bool relative);
    void setVolume(float volume);

    virtual void setLooping(bool looping) = 0;
    virtual void setOffset(duration_t offset) = 0;

    virtual duration_t offset() const = 0;
    virtual bool looped() const = 0;

    const duration_t &duration() const;
    float volume() const;
    const Vector3<float> &position() const;
    bool playing() const;
    AudioStatus status() const;
    bool valid() const;

protected:
    struct BufferWrapper {
        std::uint32_t buffer = 0;
        BufferWrapper();
        ~BufferWrapper();
    };

    explicit AudioSource(std::unique_ptr<AudioFile> filePtr, std::uint32_t newSource);

    std::uint32_t m_source;
    std::unique_ptr<AudioFile> m_file;
};

#endif // AUDIO_SOURCE_H
