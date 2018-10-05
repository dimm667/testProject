#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.h"

Image::Image(const std::string &pathToFile)
{
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(pathToFile.c_str(), &width, &height, &nrChannels, 0);
    if(!data)
    {
        stbi_image_free(data);
        throw std::runtime_error(std::string("ERROR::TEXTURE::LOAD\n") + std::string("Failed to load texture: ") + pathToFile + std::string("\n"));
    }
}

float Image::get(PixelDataType type, unsigned int x, unsigned int y)
{
    return static_cast<float>(static_cast<unsigned char *>(data)[(x * width + y) * nrChannels + static_cast<unsigned int>(type)]) / 255.0f;
}

Image::~Image()
{
    stbi_image_free(data);
}

Texture::Texture(const std::string & pathToFile) :
    bindedToUnit(GL_TEXTURE0)
{
    glGenTextures(1, &texID);

    if(texID == GL_INVALID_VALUE )
    {
        throw std::runtime_error(std::string("ERROR::TEXTURE::LOAD\n") + std::string("Max textures number reached. Falid to load: ") + pathToFile + std::string("\n"));
    }

    GLint width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(pathToFile.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format;
        switch (nrChannels)
        {
        case 1: format = GL_RED; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: break;
        }

        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    else
    {
        stbi_image_free(data);
        throw std::runtime_error(std::string("ERROR::TEXTURE::LOAD\n") + std::string("Failed to load texture: ") + pathToFile + std::string("\n"));
    }
}

int Texture::getBindUnit() const
{
    return static_cast<int>(bindedToUnit);
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, texID);
}

void Texture::activate(GLenum textureUint)
{
    bindedToUnit = textureUint;
    glActiveTexture(textureUint);
    bind();
}

Texture::~Texture()
{
    // TODO: Release texture info from OpenGL.
    //glDeleteTextures(1, &texID);
}

TextureCubeMap::TextureCubeMap(const std::vector<std::string> & pathsToFiles)
{
    glGenTextures(1, &texID);
    if(texID == GL_INVALID_VALUE )
    {
        throw std::runtime_error(std::string("ERROR::TEXTURE_CUBE_MAP::LOAD\n") + std::string("Max textures number reached.\n"));
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);
    int texIdx = 0;
    for(auto && pathToFile : pathsToFiles)
    {
        unsigned char *data = stbi_load(pathToFile.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format;
            switch (nrChannels)
            {
            case 1: format = GL_RED; break;
            case 3: format = GL_RGB; break;
            case 4: format = GL_RGBA; break;
            default: break;
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (texIdx++),
                         0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            stbi_image_free(data);
            throw std::runtime_error(std::string("ERROR::TEXTURE_CUBE_MAP::LOAD\n") + std::string("Failed to load texture: ") + pathToFile + std::string("\n"));
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void TextureCubeMap::bind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
}

