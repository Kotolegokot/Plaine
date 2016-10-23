#include "Randomizer.h"

std::default_random_engine Randomizer::engine { std::random_device()() };
