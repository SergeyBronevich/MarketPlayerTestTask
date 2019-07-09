#pragma once

namespace MarketPlayerTestTask
{
    class Image;

    class MultithreadGaussianBlur
    {
    private:
        MultithreadGaussianBlur();

    public:
        static bool gaussianBlur(const Image& image, int kernelSize,
                                 double sigma, Image& gaussianBlurImage,
                                 int numberOfThreads);
    };
} // namespace MarketPlayerTestTask
