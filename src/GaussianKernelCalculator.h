#pragma once

namespace MarketPlayerTestTask
{
    class Kernel;

    class GaussianKernelCalculator
    {
    private:
        GaussianKernelCalculator();

    public:
        static void calculate(Kernel& kernel, double sigma);
    };
} // namespace MarketPlayerTestTask
