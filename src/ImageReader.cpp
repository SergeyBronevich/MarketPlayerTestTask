#include "ImageReader.h"
#include <memory>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <cstring>

namespace MarketPlayerTestTask
{
    bool ImageReader::read(const char* filePath, Image::Format format,
                           Image& image)
    {
        int width = 0;
        int height = 0;
        int channelsInFile = 0;
        int desiredChannels =
            (format == Image::Format::Grayscale) ? STBI_grey : STBI_rgb;

        stbi_uc* stbImage = ::stbi_load(filePath, &width, &height,
                                        &channelsInFile, desiredChannels);

        if (!stbImage)
            return false;

        if (!image.create(width, height, format))
        {
            ::stbi_image_free(stbImage);

            return false;
        }

        std::memcpy(image.getPixels(), stbImage, width * height * format);

        ::stbi_image_free(stbImage);

        return true;
    }
} // namespace MarketPlayerTestTask
