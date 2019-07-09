#include "MultithreadGaussianBlur.h"

#include <vector>
#include <thread>
#include <cassert>
#include <iostream>

#include "Image.h"
#include "Kernel.h"
#include "GaussianKernelCalculator.h"
#include "CoordinateConverter.h"
#include "DoubleToByteTruncater.h"

namespace MarketPlayerTestTask
{
    struct _GaussianBlurThreadParams
    {
        struct Shared
        {
            const Image* image;
            const Kernel* kernel;
            Image* gaussianBlurredImage;
        };

        Shared* shared;
        int beginPixelIndex;
        int endPixelIndex;
    };

    static bool _canProcessPixelByKernel(int kernelSizeDiv2, int width,
                                         int height, int x, int y)
    {
        if (x < kernelSizeDiv2)
        {
            return false;
        }
        else if (x > width - kernelSizeDiv2 - 1)
        {
            return false;
        }
        else if (y < kernelSizeDiv2)
        {
            return false;
        }
        else if (y > height - kernelSizeDiv2 - 1)
        {
            return false;
        }

        return true;
    }

    static void _gaussianBlurThreadFunc(_GaussianBlurThreadParams* threadParams)
    {
        assert(threadParams->shared->image);
        assert(threadParams->shared->kernel);
        assert(threadParams->shared->gaussianBlurredImage);

        // std::cout << "threadParams->beginPixelIndex: "
        //          << threadParams->beginPixelIndex << std::endl;
        // std::cout << "threadParams->endPixelIndex: "
        //          << threadParams->endPixelIndex << std::endl;

        int kernelSize = threadParams->shared->kernel->getSize();
        int kernelSizeDiv2 = threadParams->shared->kernel->getSize() / 2;

        for (int pixelIndex = threadParams->beginPixelIndex;
             pixelIndex < threadParams->endPixelIndex; ++pixelIndex)
        {
            int imageX = 0;
            int imageY = 0;

            CoordinateConverter::convert1DTo2D(
                pixelIndex, threadParams->shared->image->getWidth(), imageX,
                imageY);

            if (!_canProcessPixelByKernel(
                    kernelSizeDiv2, threadParams->shared->image->getWidth(),
                    threadParams->shared->image->getHeight(), imageX, imageY))
            {
                continue;
            }

            double accumulatorGray = 0.0;

            for (int kernelX = 0; kernelX < kernelSize; ++kernelX)
            {
                for (int kernelY = 0; kernelY < kernelSize; ++kernelY)
                {
                    int pixelX = imageX - kernelSizeDiv2 + kernelX;
                    int pixelY = imageY - kernelSizeDiv2 + kernelY;

                    const uint8_t* pixel =
                        threadParams->shared->image->getPixel(pixelX, pixelY);

                    double kernelValue =
                        threadParams->shared->kernel->get(kernelX, kernelY);

                    accumulatorGray += *pixel * kernelValue;
                }
            }

            uint8_t* newPixel =
                threadParams->shared->gaussianBlurredImage->getPixel(imageX,
                                                                     imageY);

            *newPixel = DoubleToByteTruncater::truncate(accumulatorGray);
        }
    }

    // Based on https://habrahabr.ru/post/142818/,
    // https://en.wikipedia.org/wiki/Kernel_(image_processing),
    // https://en.wikipedia.org/wiki/Gaussian_blur.
    bool MultithreadGaussianBlur::gaussianBlur(const Image& image,
                                               int kernelSize, double sigma,
                                               Image& gaussianBlurredImage,
                                               int numberOfThreads)
    {
        Kernel kernel(kernelSize);

        GaussianKernelCalculator::calculate(kernel, sigma);

        if (!gaussianBlurredImage.create(image.getWidth(), image.getHeight(),
                                         image.getFormat()))
        {
            return false;
        }

        int totalPixels = image.getWidth() * image.getHeight();

        int numberOfPixelsToProcessByOneThread = totalPixels / numberOfThreads;

        _GaussianBlurThreadParams::Shared sharedThreadsParams;

        sharedThreadsParams.image = &image;
        sharedThreadsParams.kernel = &kernel;
        sharedThreadsParams.gaussianBlurredImage = &gaussianBlurredImage;

        std::vector<_GaussianBlurThreadParams> threadsParams;
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
            threads.push_back(std::thread(_gaussianBlurThreadFunc,
                                          &threadsParams[threadIndex]));
        }

        // Wait all threads
        for (auto& thread : threads)
            thread.join();

        return true;
    }

} // namespace MarketPlayerTestTask
