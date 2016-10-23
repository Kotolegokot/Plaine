#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <random>

class Randomizer
{
public:
    Randomizer() = delete;

    template <typename RealType> static RealType getReal(RealType min = 0, RealType max = 1)
    {
        std::uniform_real_distribution<RealType> distribution(min, max);

        return distribution(engine);
    }

    template <typename IntType> static IntType getInteger(IntType min = 0, IntType max = std::numeric_limits<IntType>::max())
    {
        std::uniform_int_distribution<IntType> distribution(min, max);

        return distribution(engine);
    }

    static int getInt(int min = 0, int max = std::numeric_limits<int>::max())
    {
        return getInteger<int>(min, max);
    }

    static float getFloat(float min = 0, float max = 1)
    {
        return getReal<float>(min, max);
    }
private:
    static std::default_random_engine engine;
};

#endif // RANDOMIZER_H
