/*

    The MIT License (MIT)

    Copyright (c) 2014 by Jakob Larsson
    Copyright (c) 2016 by Kotik Andreev

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

#include "audio/AudioSource.h"

AudioSource::AudioSource(std::unique_ptr<AudioFile> filePtr, uint32_t newSource) :
    m_source(newSource), m_file(std::move(filePtr))
{
    m_valid = m_file->open();

    unsigned int sampleCount = m_file->fileInfo().frames * m_file->fileInfo().channels;

    m_duration = duration_t(static_cast<float>(sampleCount) /
                            static_cast<float>(m_file->fileInfo().samplerate) /
                            static_cast<float>(m_file->fileInfo().channels));
}

AudioSource::~AudioSource()
{
    if (alIsSource(m_source) && m_valid)
        alDeleteSources(1, &m_source);
}

ALuint AudioSource::invalidate()
{
    stop();
    m_file->close();
    m_valid = false;

    ALuint tempSource = m_source;
    m_source = 0;

    return tempSource;
}

void AudioSource::setPitch(float pitch)
{
    if (m_valid)
        alSourcef(m_source, AL_PITCH, pitch);
}

void AudioSource::setGain(float volume)
{
    if (m_valid)
        alSourcef(m_source, AL_GAIN, volume);
}

void AudioSource::setMaxDistance(float maxDistance)
{
    if (m_valid)
        alSourcef(m_source, AL_MAX_DISTANCE, maxDistance);
}

void AudioSource::setRolloffFactor(float rolloffFactor)
{
    if (m_valid)
        alSourcef(m_source, AL_ROLLOFF_FACTOR, rolloffFactor);
}

void AudioSource::setReferenceDistance(float referenceDistance)
{
    if (m_valid)
        alSourcef(m_source, AL_REFERENCE_DISTANCE, referenceDistance);
}

void AudioSource::setMinGain(float minGain)
{
    if (m_valid)
        alSourcef(m_source, AL_MIN_GAIN, minGain);
}

void AudioSource::setMaxGain(float maxGain)
{
    if (m_valid)
        alSourcef(m_source, AL_MAX_GAIN, maxGain);
}

void AudioSource::setConeOuterGain(float coneOuterGain)
{
    if (m_valid)
        alSourcef(m_source, AL_CONE_OUTER_GAIN, coneOuterGain);
}

void AudioSource::setConeInnerAngle(float coneInnerAngle)
{
    if (m_valid)
        alSourcef(m_source, AL_CONE_INNER_ANGLE, coneInnerAngle);
}

void AudioSource::setConeOuterAngle(float coneOuterAngle)
{
    if (m_valid)
        alSourcef(m_source, AL_CONE_OUTER_ANGLE, coneOuterAngle);
}

void AudioSource::setPosition(const Vector3<float> &position)
{
    if (m_valid)
        alSource3f(m_source, AL_POSITION, position.x, position.y, position.z);
}

void AudioSource::setVelocity(const Vector3<float> &velocity)
{
    if (m_valid)
        alSource3f(m_source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void AudioSource::setDirection(const Vector3<float> &direction)
{
    if (m_valid)
        alSource3f(m_source, AL_DIRECTION, direction.x, direction.y, direction.z);
}

void AudioSource::setRelativeToListener(bool relative)
{
    if (m_valid)
        alSourcei(m_source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
}

void AudioSource::setType(Type type)
{
    if (m_valid) {
        ALint alType;
        switch (type) {
        case Type::Undetermined:
            alType = AL_UNDETERMINED;
            break;
        case Type::Static:
            alType = AL_STATIC;
            break;
        case Type::Streaming:
            alType = AL_STREAMING;
            break;
        }

        alSourcei(m_source, AL_SOURCE_TYPE, alType);
    }
}

float AudioSource::pitch() const
{
    if (!m_valid)
        return 0;

    ALfloat result;
    alGetSourcef(m_source, AL_PITCH, &result);
    return result;
}

float AudioSource::gain() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALfloat result;
    alGetSourcef(m_source, AL_GAIN, &result);
    return result;
}

float AudioSource::maxDistance() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALfloat result;
    alGetSourcef(m_source, AL_MAX_DISTANCE, &result);
    return result;
}

float AudioSource::rolloffFactor() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALfloat result;
    alGetSourcef(m_source, AL_ROLLOFF_FACTOR, &result);
    return result;
}

float AudioSource::referenceDistance() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALfloat result;
    alGetSourcef(m_source, AL_REFERENCE_DISTANCE, &result);
    return result;
}

float AudioSource::minGain() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALfloat result;
    alGetSourcef(m_source, AL_MIN_GAIN, &result);
    return result;
}

float AudioSource::maxGain() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALfloat result;
    alGetSourcef(m_source, AL_MAX_GAIN, &result);
    return result;
}

float AudioSource::coneOuterGain() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALfloat result;
    alGetSourcef(m_source, AL_CONE_OUTER_GAIN, &result);
    return result;
}

float AudioSource::coneInnerAngle() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALfloat result;
    alGetSourcef(m_source, AL_CONE_INNER_ANGLE, &result);
    return result;
}

float AudioSource::coneOuterAngle() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALfloat result;
    alGetSourcef(m_source, AL_CONE_OUTER_ANGLE, &result);
    return result;
}

Vector3<float> AudioSource::position() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    Vector3<float> result;
    alGetSource3f(m_source, AL_POSITION, &result.x, &result.y, &result.z);
    return result;
}

Vector3<float> AudioSource::velocity() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    Vector3<float> result;
    alGetSource3f(m_source, AL_VELOCITY, &result.x, &result.y, &result.z);
    return result;
}

Vector3<float> AudioSource::direction() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    Vector3<float> result;
    alGetSource3f(m_source, AL_DIRECTION, &result.x, &result.y, &result.z);
    return result;
}

bool AudioSource::relativeToListener() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALint result;
    alGetSourcei(m_source, AL_SOURCE_RELATIVE, &result);
    return result == AL_TRUE;
}

AudioSource::Type AudioSource::type() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALint result;
    alGetSourcei(m_source, AL_SOURCE_TYPE, &result);
    switch (result) {
    case AL_STATIC:
        return Type::Static;
    case AL_STREAMING:
        return Type::Streaming;
    case AL_UNDETERMINED:
    default:
        return Type::Undetermined;
    }
}

bool AudioSource::looping() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALint result;
    alGetSourcei(m_source, AL_LOOPING, &result);
    return result;
}

unsigned int AudioSource::bufferID() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALint result;
    alGetSourcei(m_source, AL_BUFFER, &result);
    return result;
}

AudioSource::State AudioSource::state() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALint result;
    alGetSourcei(m_source, AL_SOURCE_STATE, &result);
    switch (result) {
    case AL_PLAYING:
        return State::Playing;
    case AL_PAUSED:
        return State::Paused;
    case AL_STOPPED:
    default:
        return State::Stopped;
    }
}

bool AudioSource::playing() const
{
    return state() == State::Playing;
}

int AudioSource::buffersQueued() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALint result;
    alGetSourcei(m_source, AL_BUFFERS_QUEUED, &result);
    return result;
}

int AudioSource::buffersProcessed() const
{
    if (!m_valid)
        throw invalid_audiosource(m_source);

    ALint result;
    alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &result);
    return result;
}

bool AudioSource::valid() const
{
    return m_valid;
}

const duration_t &AudioSource::duration() const
{
    return m_duration;
}

AudioSource::BufferWrapper::BufferWrapper()
{
    alGenBuffers(1, &buffer);
}

AudioSource::BufferWrapper::~BufferWrapper()
{
    alDeleteBuffers(1, &buffer);
}
