#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>

class Audio {
    Audio() {
        collision.openFromFile("media/sounds/collision.ogg");
        // background.openFromFile("media/sounds/background.ogg");
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
