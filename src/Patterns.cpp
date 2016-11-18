#include "Patterns.h"

constexpr std::size_t Patterns::ALL;
constexpr std::size_t Patterns::CRYSTALS;

const std::array<std::shared_ptr<IObstaclePattern>, Patterns::ALL> Patterns::all = {
    {
        std::make_shared<Crystal<1, 2>>(0),
        std::make_shared<Crystal<1, 4>>(1),
        std::make_shared<Crystal<2, 4>>(2),
        std::make_shared<Crystal<3, 8>>(3),
        std::make_shared<Tunnel>(4),
        std::make_shared<Alley<5>>(5)
    }
};

const std::array<std::shared_ptr<IObstaclePattern>, Patterns::CRYSTALS> Patterns::crystals = {
    {
        Patterns::all[0],
        Patterns::all[1],
        Patterns::all[2],
        Patterns::all[3]
    }
};
