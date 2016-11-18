#include "ObstaclePatternFactory.h"

constexpr std::size_t Patterns::ALL;
constexpr std::size_t Patterns::SMALL;

const std::array<std::shared_ptr<IObstaclePattern>, Patterns::ALL> Patterns::all = {
    {
        std::make_shared<Crystal>(0),
        std::make_shared<Tunnel>(1),
        std::make_shared<Alley<5>>(2)
    }
};

const std::array<std::shared_ptr<IObstaclePattern>, Patterns::SMALL> Patterns::small = {
    {
        Patterns::all[0], // Crystal
    }
};
