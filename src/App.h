#pragma once
#include <string>
#include "Image.h"

namespace MarketPlayerTestTask
{
    // Main application class
    class App
    {
    private:
        App();

    public:
        int main(int argc, char** argv);

    private:
        bool _parseCmdLine(int argc, char** argv);

    public:
        bool _readInputImage();

    public:
        bool _convertInputImageToGrayscale();

    public:
        bool _gaussianBlurGrayscaleImage();

    private:
        void _formatOutputGrayscaleImageFilePath();

    public:
        bool _writeGrayscaleImage();

    private:
        void _formatGaussianBluredImageFilePath();

    public:
        bool _writeGaussianBluredImage();

    private:
        std::string _inputImageFilePath;
        int _numberOfThreads;
        int _kernelSize;
        double _sigma;
        Image _inputImage; // Input RGB image
        std::string _inputImageFilePathWithoutExtention;
        Image _grayscaleImage;
        Image _gaussianBlurredImage;
        std::string _grayscaleImageFilePath;
        std::string _gaussianBlurredImageFilePath;

    public:
        static App& getInstance();

    private:
        static App _app;
    };
} // namespace MarketPlayerTestTask
