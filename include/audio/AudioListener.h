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

#ifndef AUDIO_LISTENER_H
#define AUDIO_LISTENER_H

#include <al.h>
#include "util/Vector3.h"

class AudioListener {
    static Vector3<float> listenerPosition;
    static Vector3<float> listenerDirection;
    static Vector3<float> listenerUpVector;

    static float listenerVolume;
public:
    static void setVolume(float volume);
    static void setPosition(const Vector3<float> &position);

    static void setDirection(const Vector3<float> &direction);
    static void setUpVector(const Vector3<float> &up);

    static float getVolume();
    static const Vector3<float> &position();
};

#endif // AUDIO_LISTENER_H