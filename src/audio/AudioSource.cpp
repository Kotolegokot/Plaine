#include "audio/AudioSource.h"

AudioSource::AudioSource(std::unique_ptr<AudioFile> filePtr, uint32_t newSource) :
    m_source(newSource), m_file(std::move(filePtr))
{
    m_isValidSource = m_file->open();

    unsigned int sampleCount = m_file->fileInfo().frames * m_file->fileInfo().channels;

    m_duration = duration_t(static_cast<float>(sampleCount) /
                            static_cast<float>(m_file->fileInfo().samplerate) /
                            static_cast<float>(m_file->fileInfo().channels));
}

AudioSource::~AudioSource()
{
    if (alIsSource(m_source) && m_isValidSource)
        alDeleteSources(1, &m_source);
}

std::uint32_t AudioSource::invalidate()
{
    stop();
    m_file->close();
    m_isValidSource = false;

    std::uint32_t tempSource = m_source;
    m_source = 0;

    return tempSource;
}

void AudioSource::setPitch(float pitch)
{
    if (m_isValidSource)
        alSourcef(m_source, AL_PITCH, pitch);
}

void AudioSource::setPosition(const Vector3<float> &position)
{
    if (m_isValidSource) {
        m_position = position;
        alSource3f(m_source, AL_POSITION, position.x, position.y, position.z);
    }
}

void AudioSource::setVelocity(const Vector3<float> &velocity)
{
    if (m_isValidSource)
        alSource3f(m_source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void AudioSource::setDirection(const Vector3<float> &direction)
{
    if (m_isValidSource)
        alSource3f(m_source, AL_DIRECTION, direction.x, direction.y, direction.z);
}

void AudioSource::setRelativeToListener(bool relative)
{
    if (m_isValidSource)
        alSourcei(m_source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
}

void AudioSource::setLooping(bool looping)
{
    if (m_isValidSource)
        alSourcei(m_source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
}

void AudioSource::setVolume(float volume)
{
    if (m_isValidSource)
        alSourcef(m_source, AL_GAIN, volume);
}

const duration_t &AudioSource::duration() const
{
    return m_duration;
}

float AudioSource::volume() const
{
    if (!m_isValidSource)
        return 0;

    ALfloat volume = 0;
    alGetSourcef(m_source, AL_GAIN, &volume);
    return volume;
}

const Vector3<float> &AudioSource::position() const
{
    return m_position;
}

bool AudioSource::playing() const
{
    return status() == AudioStatus::Playing;
}

AudioSource::AudioStatus AudioSource::status() const
{
    if (!m_isValidSource)
        return AudioStatus::Stopped;

    ALint state = AL_INITIAL;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    switch (state) {
    case AL_PLAYING:
        return AudioStatus::Playing;
    case AL_PAUSED:
        return AudioStatus::Paused;
    default:
        return AudioStatus::Stopped;
    }
}

bool AudioSource::valid() const
{
    return m_isValidSource;
}

AudioSource::BufferWrapper::BufferWrapper()
{
    alGenBuffers(1, &buffer);
}

AudioSource::BufferWrapper::~BufferWrapper()
{
    alDeleteBuffers(1, &buffer);
}
