#pragma once
#include "Image.h"

namespace MarketPlayerTestTask
{
    // Simple image writer
    class ImageWriter
    {
    private:
        ImageWriter();

    public:
        static bool write(const char* filePath, const Image& image);
    };
} // namespace MarketPlayerTestTask
