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

#ifndef AUDIO_FILE_H
#define AUDIO_FILE_H

#include <al.h>
#include <alc.h>
#include <sndfile.h>
#include <string>
#include <vector>
#include "Log.h"

class AudioFile {
    static ALenum getFormatFromChannels(unsigned int channelCount);

    SNDFILE *m_file = nullptr;
    SF_INFO m_fileInfo;
public:
    AudioFile() = default;
    ~AudioFile();

    bool tryOpen(const std::string &filePath);
    void close();

    bool open() const;

    void setReadingOffset(float seconds);
    bool readSecondIntoBuffer(ALuint buffer, bool allowLooping = false);
    void readFileIntoBuffer(ALuint buffer);

    const SF_INFO &fileInfo() const;
};

#endif // AUDIO_FILE_H