#pragma once
#include <vector>
#include <cassert>

namespace MarketPlayerTestTask
{
    static const int DEFAULT_KERNEL_SIZE = 3;

    class Kernel
    {
    public:
        Kernel(int kernelSize = DEFAULT_KERNEL_SIZE) : _kernelSize(kernelSize)
        {
            assert(kernelSize > 0);

            _kernel.resize(kernelSize * kernelSize);
        }

    public:
        int getSize() const
        {
            return _kernelSize;
        }
        double get(int i, int j) const
        {
            return _kernel[i + j * _kernelSize];
        }
        void set(int i, int j, double value)
        {
            _kernel[i + j * _kernelSize] = value;
        }
        void normalize()
        {
            double div = 0.0;

            for (int x = 0; x < _kernelSize; ++x)
            {
                for (int y = 0; y < _kernelSize; ++y)
                {
                    div += _kernel[x + y * _kernelSize];
                }
            }

            div = 1.0 / div;

            for (int x = 0; x < _kernelSize; ++x)
            {
                for (int y = 0; y < _kernelSize; ++y)
                {
                    _kernel[x + y * _kernelSize] *= div;
                }
            }
        }

    private:
        int _kernelSize;
        std::vector<double> _kernel;
    };
} // namespace MarketPlayerTestTask
