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

#include "audio/AudioListener.h"

Vector3<float> AudioListener::listenerPosition { 0, 0, 0 };
Vector3<float> AudioListener::listenerDirection { 0, 0, -1 };
Vector3<float> AudioListener::listenerUpVector { 0, 1, 0 };

float AudioListener::listenerVolume = 1.0f;

void AudioListener::setVolume(float volume)
{
    alListenerf(AL_GAIN, volume);
}

void AudioListener::setPosition(const Vector3<float> &position)
{
    alListener3f(AL_POSITION, position.x, position.y, position.z);
    listenerPosition = position;
}

void AudioListener::setDirection(const Vector3<float> &direction)
{
    listenerDirection = direction;
    float orientation[] = {
        listenerDirection.x,
        listenerDirection.y,
        listenerDirection.z,
        listenerUpVector.x,
        listenerUpVector.y,
        listenerUpVector.z
    };
    alListenerfv(AL_ORIENTATION, orientation);
}

void AudioListener::setUpVector(const Vector3<float> &up)
{
    listenerUpVector = up;
    setDirection(listenerDirection);
}

const Vector3<float> &AudioListener::getPosition()
{
    return listenerPosition;
}
