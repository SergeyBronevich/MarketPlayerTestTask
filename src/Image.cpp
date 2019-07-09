#include "Image.h"
#include <cassert>
#include <memory>
#include <cstring>

namespace MarketPlayerTestTask
{
    Image::Image()
        : _width(0), _height(0), _format(Format::Unknown), _pixels(nullptr)
    {
    }

    Image::~Image()
    {
        if (_pixels)
            std::free(_pixels);
    }

    bool Image::create(int width, int height, Format format)
    {
        assert(!_pixels);
        assert(format != Format::Unknown);

        size_t pixelsSize = width * height * format;

        _pixels = static_cast<uint8_t*>(std::malloc(pixelsSize));
        std::memset(_pixels, 0, pixelsSize);

        if (!_pixels)
            return false;

        _width = width;
        _height = height;
        _format = format;

        return true;
    }

    bool Image::destroy()
    {
        assert(_pixels);

        std::free(_pixels);
        _pixels = nullptr;

        return true;
    }

    const int Image::getWidth() const
    {
        return _width;
    }

    const int Image::getHeight() const
    {
        return _height;
    }

    const Image::Format Image::getFormat() const
    {
        return _format;
    }

    uint8_t* Image::getPixels()
    {
        return _pixels;
    }

    const uint8_t* Image::getPixels() const
    {
        return _pixels;
    }

    uint8_t* Image::getPixel(int x, int y)
    {
        return &_pixels[(y * _width + x) * _format];
    }

    const uint8_t* Image::getPixel(int x, int y) const
    {
        return &_pixels[(y * _width + x) * _format];
    }
} // namespace MarketPlayerTestTask
