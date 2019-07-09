#define _USE_MATH_DEFINES
#include <cassert>
#include <cmath>
#include "GaussianKernelCalculator.h"
#include "Kernel.h"

namespace MarketPlayerTestTask
{
    // Based on
    // https://softwarebydefault.com/2013/06/08/calculating-gaussian-kernels/.
    void GaussianKernelCalculator::calculate(Kernel& kernel, double sigma)
    {
        assert(kernel.getSize() > 0);

        double twoSigmaInSquare = 2.0 * sigma * sigma;

        int kernelSizeDiv2 = (int)std::ceil(kernel.getSize() / 2);

        for (int x = 0; x < kernel.getSize(); ++x)
        {
            int kernelX = std::abs(x - kernelSizeDiv2);

            for (int y = 0; y < kernel.getSize(); ++y)
            {
                int kernelY = std::abs(y - kernelSizeDiv2);

                double e =
                    std::pow(M_E, -((kernelX * kernelX + kernelY * kernelY) /
                                    twoSigmaInSquare));

                double value = (1.0 / (M_PI * twoSigmaInSquare)) * e;

                kernel.set(x, y, value);
            }
        }

        kernel.normalize();
    }
} // namespace MarketPlayerTestTask
