#pragma once
#include "Image.h"

namespace MarketPlayerTestTask
{
    // Simple image reader
    class ImageReader
    {
    private:
        ImageReader();

    public:
        static bool read(const char* filePath, Image::Format format,
                         Image& image);
    };

} // namespace MarketPlayerTestTask
