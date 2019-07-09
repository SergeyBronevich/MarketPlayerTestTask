#include "MultithreadRGBToGrayscaleImageConverter.h"
#include "Image.h"
#include "CoordinateConverter.h"

#include <cassert>
#include <vector>
#include <thread>

namespace MarketPlayerTestTask
{
    struct _RGBToGrayscaleImageConverterThreadParams
    {
        struct Shared
        {
            const Image* rgbImage;
            Image* grayscaleImage;
        };

        Shared* shared;
        int beginPixelIndex;
        int endPixelIndex;
    };

    static void _RGBToGrayscaleImageConverterThreadFunc(
        _RGBToGrayscaleImageConverterThreadParams* threadParams)
    {
        assert(threadParams->shared->rgbImage);
        assert(threadParams->shared->grayscaleImage);

        for (int pixelIndex = threadParams->beginPixelIndex;
             pixelIndex < threadParams->endPixelIndex; ++pixelIndex)
        {
            int imageX = 0;
            int imageY = 0;

            CoordinateConverter::convert1DTo2D(
                pixelIndex, threadParams->shared->rgbImage->getWidth(), imageX,
                imageY);

            const uint8_t* rgbImagePixel =
                threadParams->shared->rgbImage->getPixel(imageX, imageY);

            uint8_t* newPixel =
                threadParams->shared->grayscaleImage->getPixel(imageX, imageY);

            *newPixel = (uint8_t)(
                ((int)rgbImagePixel[Image::PixelChanels::Green] * 150 +
                 (int)rgbImagePixel[Image::PixelChanels::Blue] * 29 +
                 (int)rgbImagePixel[Image::PixelChanels::Red] * 77) >>
                8);
        }
    }

    bool MultithreadRGBToGrayscaleImageConverter::convert(const Image& rgbImage,
                                                          Image& grayscaleImage,
                                                          int numberOfThreads)
    {
        if (!grayscaleImage.create(rgbImage.getWidth(), rgbImage.getHeight(),
                                   Image::Format::Grayscale))
        {
            return false;
        }

        int totalPixels = rgbImage.getWidth() * rgbImage.getHeight();

        int numberOfPixelsToProcessByOneThread = totalPixels / numberOfThreads;

        _RGBToGrayscaleImageConverterThreadParams::Shared sharedThreadsParams;

        sharedThreadsParams.rgbImage = &rgbImage;
        sharedThreadsParams.grayscaleImage = &grayscaleImage;

        std::vector<_RGBToGrayscaleImageConverterThreadParams> threadsParams;
        threadsParams.resize(numberOfThreads);

        std::vector<std::thread> threads;

        int pixelIndex = 0;

        for (int threadIndex = 0; threadIndex < numberOfThreads - 1;
             ++threadIndex)
        {
            threadsParams[threadIndex].shared = &sharedThreadsParams;

            threadsParams[threadIndex].beginPixelIndex = pixelIndex;
            pixelIndex += numberOfPixelsToProcessByOneThread;
            threadsParams[threadIndex].endPixelIndex = pixelIndex;
        }

        size_t lastThreadIndex = numberOfThreads - 1;

        threadsParams[lastThreadIndex].shared = &sharedThreadsParams;
        threadsParams[lastThreadIndex].beginPixelIndex = pixelIndex;
        threadsParams[lastThreadIndex].endPixelIndex = totalPixels;

        // Run all threads
        for (int threadIndex = 0; threadIndex < numberOfThreads; ++threadIndex)
        {
            threads.push_back(
                std::thread(_RGBToGrayscaleImageConverterThreadFunc,
                            &threadsParams[threadIndex]));
        }

        // Wait all threads
        for (auto& thread : threads)
            thread.join();

        return true;
    }
} // namespace MarketPlayerTestTask
