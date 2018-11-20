#include "imageloader.h"
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.h"

ImageLoader::ImageLoader(const std::string & pathToFile)
{
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(pathToFile.c_str(), &width, &height, &nrChannels, 0);
    if(data == nullptr)
    {
        stbi_image_free(data);
        throw std::runtime_error(std::string("ERROR::TEXTURE::LOAD\n") + std::string("Failed to load texture: ") + pathToFile + std::string("\n"));
    }
}

ImageLoader::~ImageLoader()
{
    stbi_image_free(data);
}

float ImageLoader::getPixel(PixelDataType type, unsigned int x, unsigned int y)
{
    return static_cast<float>(static_cast<unsigned char *>(data)[(x * width + y) * nrChannels + static_cast<unsigned int>(type)]) / 255.0f;
}
