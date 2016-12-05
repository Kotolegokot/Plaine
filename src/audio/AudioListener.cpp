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

#include "audio/AudioListener.h"

void AudioListener::setGain(float volume)
{
    alListenerf(AL_GAIN, volume);
}

void AudioListener::setPosition(const Vector3<float> &position)
{
    alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void AudioListener::setVelocity(const Vector3<float> &velocity)
{
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void AudioListener::setDirection(const Vector3<float> &direction)
{
    ALfloat orientation[6];
    alGetListenerfv(AL_ORIENTATION, orientation);

    orientation[0] = direction.x;
    orientation[1] = direction.y;
    orientation[2] = direction.z;
    alListenerfv(AL_ORIENTATION, orientation);
}

void AudioListener::setUpVector(const Vector3<float> &upVector)
{
    ALfloat orientation[6];
    alGetListenerfv(AL_ORIENTATION, orientation);

    orientation[3] = upVector.x;
    orientation[4] = upVector.y;
    orientation[5] = upVector.z;
    alListenerfv(AL_ORIENTATION, orientation);
}

float AudioListener::gain()
{
    ALfloat gain;
    alGetListenerf(AL_GAIN, &gain);

    return gain;
}

Vector3<float> AudioListener::position()
{
    Vector3<float> result;
    alGetListener3f(AL_POSITION, &result.x, &result.y, &result.z);

    return result;
}

Vector3<float> AudioListener::velocity()
{
    Vector3<float> result;
    alGetListener3f(AL_VELOCITY, &result.x, &result.y, &result.z);

    return result;
}

Vector3<float> AudioListener::direction()
{
    ALfloat orientation[6];
    alGetListenerfv(AL_ORIENTATION, orientation);

    return { orientation[0], orientation[1], orientation[2] };
}

Vector3<float> AudioListener::upVector()
{
    ALfloat orientation[6];
    alGetListenerfv(AL_ORIENTATION, orientation);

    return { orientation[3], orientation[4], orientation[5] };
}
