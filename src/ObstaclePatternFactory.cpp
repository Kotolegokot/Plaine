#include "ObstaclePatternFactory.h"

constexpr std::size_t ObstaclePatternFactory::PATTERNS_COUNT;

const std::array<std::unique_ptr<IObstaclePattern>, ObstaclePatternFactory::PATTERNS_COUNT>
    ObstaclePatternFactory::patterns = { {
        std::make_unique<Crystal>(),
        std::make_unique<Tunnel>(),
        std::make_unique<Valley<5>>()
} };
