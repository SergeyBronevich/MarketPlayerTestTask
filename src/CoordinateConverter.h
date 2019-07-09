#pragma once

namespace MarketPlayerTestTask
{
    class CoordinateConverter
    {
    private:
        CoordinateConverter();

    public:
        static void convert1DTo2D(int index, int width, int& x, int& y)
        {
            x = index % width;
            y = index / width;
        }
    };
} // namespace MarketPlayerTestTask
