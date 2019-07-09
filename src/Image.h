#pragma once
#include <cinttypes>

namespace MarketPlayerTestTask
{
    // Simple image class
    class Image
    {
    public:
        enum Format
        {
            Unknown = -1,
            Grayscale = 1,
            RGB = 3
        };

        enum PixelChanels
        {
            Red = 0,
            Green = 1,
            Blue = 2
        };

    public:
        Image();
        ~Image();

    public:
        bool create(int width, int height, Format format);
        bool destroy();

    public:
        const int getWidth() const;
        const int getHeight() const;

    public:
        const Format getFormat() const;

    public:
        uint8_t* getPixels();
        const uint8_t* getPixels() const;

    public:
        uint8_t* getPixel(int x, int y);
        const uint8_t* getPixel(int x, int y) const;

    private:
        int _width;
        int _height;
        Format _format;
        uint8_t* _pixels;
    };
} // namespace MarketPlayerTestTask
