#include "ImageWriter.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

namespace MarketPlayerTestTask
{
    bool ImageWriter::write(const char* filePath, const Image& image)
    {
        if (!stbi_write_png(filePath, image.getWidth(), image.getHeight(),
                            image.getFormat(), image.getPixels(),
                            image.getWidth() * image.getFormat()))
        {
            return false;
        }

        return true;
    }
} // namespace MarketPlayerTestTask
