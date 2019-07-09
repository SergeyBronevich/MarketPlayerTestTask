#include "App.h"
#include <iostream>
#include "Image.h"
#include "ImageReader.h"
#include "MultithreadRGBToGrayscaleImageConverter.h"
#include "MultithreadGaussianBlur.h"
#include "ImageWriter.h"
#include <thread>

namespace MarketPlayerTestTask
{
    App App::_app;

    App::App()
    {
    }

    App& App::getInstance()
    {
        return _app;
    }

    int App::main(int argc, char** argv)
    {
        if (!_parseCmdLine(argc, argv))
            return -1;

        if (!_readInputImage())
            return -1;

        std::chrono::time_point<std::chrono::system_clock> start =
            std::chrono::system_clock::now();

        if (!_convertInputImageToGrayscale())
            return -1;

        if (!_gaussianBlurGrayscaleImage())
            return -1;

        std::chrono::time_point<std::chrono::system_clock> end =
            std::chrono::system_clock::now();

        int64_t elapsedMilliseconds =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();

        std::cout << "Elapsed milliseconds: " << elapsedMilliseconds
                  << std::endl;

        if (!_writeGrayscaleImage())
            return -1;

        if (!_writeGaussianBluredImage())
            return -1;

        return 0;
    }

    static const int INPUT_IMAGE_FILE_PATH_ARG_INDEX = 1;
    static const char* DEFAULT_INPUT_IMAGE_FILE_PATH = "lena512color.bmp";
    static const int NUMBER_OF_THREADS_ARG_INDEX = 2;
    static const int KERNEL_SIZE_ARG_INDEX = 3;
    static const int DEFAULT_KERNEL_SIZE = 9;
    static const int SIGMA_ARG_INDEX = 4;
    static const double DEFAULT_SIGMA = 2.0;

    bool App::_parseCmdLine(int argc, char** argv)
    {
        if (argc > 1)
            _inputImageFilePath = argv[INPUT_IMAGE_FILE_PATH_ARG_INDEX];
        else
            _inputImageFilePath = DEFAULT_INPUT_IMAGE_FILE_PATH;

        std::cout << "Input image file path: " << _inputImageFilePath.c_str()
                  << std::endl;

        size_t dot = _inputImageFilePath.find_last_of(".");

        if (dot == std::string::npos)
            return false;

        _inputImageFilePathWithoutExtention =
            _inputImageFilePath.substr(0, dot);

        if (argc > 2)
        {
            _numberOfThreads = std::atoi(argv[NUMBER_OF_THREADS_ARG_INDEX]);
        }
        else
        {
            _numberOfThreads = std::thread::hardware_concurrency();
        }

        std::cout << "Number of threads: " << _numberOfThreads << std::endl;

        if (argc > 3)
        {
            _kernelSize = std::atoi(argv[KERNEL_SIZE_ARG_INDEX]);
        }
        else
        {
            _kernelSize = DEFAULT_KERNEL_SIZE;
        }

        std::cout << "Kernel size: " << _kernelSize << std::endl;

        if (argc > 4)
        {
            _sigma = std::atof(argv[SIGMA_ARG_INDEX]);
        }
        else
        {
            _sigma = DEFAULT_SIGMA;
        }

        std::cout << "Sigma: " << _sigma << std::endl;

        return true;
    }

    bool App::_readInputImage()
    {
        if (!ImageReader::read(_inputImageFilePath.c_str(), Image::Format::RGB,
                               _inputImage))
        {
            std::cerr << "Failed to read input image from file "
                      << _inputImageFilePath.c_str() << std::endl;

            return false;
        }

        return true;
    }

    bool App::_convertInputImageToGrayscale()
    {
        if (!MultithreadRGBToGrayscaleImageConverter::convert(
                _inputImage, _grayscaleImage, _numberOfThreads))
        {
            std::cerr << "Failed to convert input image to grayscale "
                      << _inputImageFilePath.c_str() << std::endl;

            return false;
        }

        return true;
    }

    bool App::_gaussianBlurGrayscaleImage()
    {
        if (!MultithreadGaussianBlur::gaussianBlur(
                _grayscaleImage, _kernelSize, _sigma, _gaussianBlurredImage,
                _numberOfThreads))
        {
            std::cerr << "Failed to gaussian blur grayscale image"
                      << _inputImageFilePath.c_str() << std::endl;

            return false;
        }

        return true;
    }

    void App::_formatOutputGrayscaleImageFilePath()
    {
        _grayscaleImageFilePath =
            _inputImageFilePathWithoutExtention + "_greyscale.png";
    }

    bool App::_writeGrayscaleImage()
    {
        _formatOutputGrayscaleImageFilePath();

        if (!ImageWriter::write(_grayscaleImageFilePath.c_str(),
                                _grayscaleImage))
        {
            std::cerr << "Failed to write grayscale blur image to file "
                      << _grayscaleImageFilePath.c_str() << std::endl;

            return false;
        }

        return true;
    }

    void App::_formatGaussianBluredImageFilePath()
    {
        _gaussianBlurredImageFilePath =
            _inputImageFilePathWithoutExtention + "_gaussian_blur_" +
            std::to_string(_kernelSize) + "_" + std::to_string(_sigma) + ".png";
    }

    bool App::_writeGaussianBluredImage()
    {
        _formatGaussianBluredImageFilePath();

        if (!ImageWriter::write(_gaussianBlurredImageFilePath.c_str(),
                                _gaussianBlurredImage))
        {
            std::cerr << "Failed to write gaussian blurred image to file "
                      << _gaussianBlurredImageFilePath.c_str() << std::endl;

            return false;
        }

        return true;
    }

} // namespace MarketPlayerTestTask
