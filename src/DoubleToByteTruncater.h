#pragma once
#include <cstdint>

namespace MarketPlayerTestTask
{
    class DoubleToByteTruncater
    {
    private:
        DoubleToByteTruncater();

    public:
        static uint8_t truncate(double x)
        {
            if (x < 0.0)
                return 0;

            if (x > 255.0)
                return 255;

            return static_cast<uint8_t>(x);
        }
    };
} // namespace MarketPlayerTestTask
