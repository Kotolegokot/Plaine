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

#include "audio/AudioChunk.h"

AudioChunk::AudioChunk(std::unique_ptr<AudioFile> audioPtr, std::uint32_t newSource) :
    AudioSource(std::move(audioPtr), newSource)
{
    if (valid()) {
        m_file->readFileIntoBuffer(m_buffer.buffer);
        m_file->close();
        alSourceQueueBuffers(m_source, 1, &m_buffer.buffer);
    }
}

void AudioChunk::play()
{
    if (valid() && !playing())
        alSourcePlay(m_source);
}

void AudioChunk::pause()
{
    if (valid() && playing())
        alSourcePause(m_source);
}

void AudioChunk::stop()
{
    if (valid() && playing()) {
        m_file->setReadingOffset(0);
        alSourceStop(m_source);
    }
}

void AudioChunk::setLooping(bool looping)
{
    if (valid())
        alSourcei(m_source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
}

void AudioChunk::setOffset(duration_t offset)
{
    if (valid())
        alSourcef(m_source, AL_SEC_OFFSET, offset.count());
}

duration_t AudioChunk::offset() const
{
    if (valid()) {
        ALfloat seconds = 0.0f;
        alGetSourcef(m_source, AL_SEC_OFFSET, &seconds);

        return duration_t(seconds);
    } else
        return duration_t(0.0f);
}

bool AudioChunk::lopped() const
{
    if (!valid())
        return false;

    ALint looping = 0;
    alGetSourcei(m_source, AL_LOOPING, &looping);

    return looping;
}
