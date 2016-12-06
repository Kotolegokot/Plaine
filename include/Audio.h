#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include "Log.h"

#define COLLISION_FILE "media/sounds/collision.ogg"
#define BACKGROUND_FILE "media/sounds/background.ogg"

class Audio {
    Audio() {
        if (!collision.openFromFile(COLLISION_FILE))
            Log::warning("couldn't open file '", COLLISION_FILE, "'");
        if (!background.openFromFile(BACKGROUND_FILE))
            Log::warning("couldn't open file '", BACKGROUND_FILE, "'");
    }
public:
    static Audio &getInstance()
    {
        static Audio instance;
        return instance;
    }

    sf::Music collision;
    sf::Music background;
};

#endif // AUDIO_H
