#pragma once

namespace MarketPlayerTestTask
{
    class Image;

    class MultithreadRGBToGrayscaleImageConverter
    {
    private:
        MultithreadRGBToGrayscaleImageConverter();

    public:
        static bool convert(const Image& rgbImage, Image& grayscaleImage,
                            int numberOfThreads);
    };
} // namespace MarketPlayerTestTask
