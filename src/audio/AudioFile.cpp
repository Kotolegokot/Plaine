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

#include "audio/AudioFile.h"

AudioFile::~AudioFile()
{
    close();
}

bool AudioFile::tryOpen(const std::__cxx11::string &filePath)
{
    m_file = sf_open(filePath.c_str(), SFM_READ, &m_fileInfo);

    if (!m_file) {
        Log::warning("could not open audio file '", filePath, "'");
        return false;
    }

    return true;
}

void AudioFile::close()
{
    if (m_file) {
        sf_close(m_file);
        m_file = nullptr;
    }
}

bool AudioFile::open() const
{
    return m_file != nullptr;
}

void AudioFile::setReadingOffset(float seconds)
{
    sf_seek(m_file, m_fileInfo.samplerate * seconds, SEEK_SET);
}

bool AudioFile::readSecondIntoBuffer(ALuint buffer, bool allowLooping)
{
    // read one second chunk of data
    int sampleSecondCount = m_fileInfo.channels * m_fileInfo.samplerate;
    std::vector<ALshort> fileData(sampleSecondCount);
    int readCount = sf_read_short(m_file, &fileData[0], sampleSecondCount);

    // if not enough data was read
    if (readCount < sampleSecondCount && allowLooping) {
        // move to start of file
        sf_seek(m_file, 0, SEEK_SET);

        // keep reading data until we have all the data we need
        while (readCount < sampleSecondCount) {
            int leftToRead = sampleSecondCount - readCount;
            int newReadCount = sf_read_short(m_file, fileData.data() + readCount, leftToRead);

            readCount += newReadCount;

            // keep re-reading the file until we have all the data we want
            if (newReadCount < leftToRead)
                sf_seek(m_file, 0, SEEK_SET);
        }
    }

    if (readCount > 0) {
        alBufferData(buffer,
                     AudioFile::getFormatFromChannels(m_fileInfo.channels),
                     &fileData[0],
                     sampleSecondCount * sizeof(ALshort),
                     m_fileInfo.samplerate);

        return true;
    }

    return true;
}

void AudioFile::readFileIntoBuffer(ALuint buffer)
{
    // read the whole file
    int sampleCount = m_fileInfo.frames * m_fileInfo.channels;
    std::vector<ALshort> fileData(sampleCount);
    sf_read_short(m_file, &fileData[0], sampleCount);

    alBufferData(buffer,
                 AudioFile::getFormatFromChannels(m_fileInfo.channels),
                 &fileData[0],
                 sampleCount * sizeof(ALushort),
                 m_fileInfo.samplerate);
}

const SF_INFO &AudioFile::fileInfo() const
{
    return m_fileInfo;
}

ALenum AudioFile::getFormatFromChannels(unsigned int channelCount)
{
    // find audio format based on channel count
    switch (channelCount) {
    case 1: return AL_FORMAT_MONO16;
    case 2: return AL_FORMAT_STEREO16;
    case 4: return alGetEnumValue("AL_FORMAT_QUAD16");
    case 5: return alGetEnumValue("AL_FORMAT_51CHN16");
    case 6: return alGetEnumValue("AL_FORMAT_61CHN16");
    case 7: return alGetEnumValue("AL_FORMAT_71CHN16");
    default: return 0;
    }
}
