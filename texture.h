#ifndef TEXTURE_H
#define TEXTURE_H
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>

class Image{
public:

    enum class PixelDataType
    {
        R,
        G,
        B,
        A
    };
    Image(const std::string & pathToFile);
    ~Image();

    float get(PixelDataType type, unsigned int x, unsigned int y);
    float getWidth() const {return width;}
    float getHeight() const {return height;}

private:
    void * data = nullptr;
    GLint width = 0;
    GLint height = 0;
    GLint nrChannels = 0;
};

class Texture
{
public:
    Texture(const std::string & pathToFile);

    int getBindUnit() const;
//    GLenum getName() const;
    void activate(GLenum textureUint);

    virtual ~Texture();

protected:
    Texture() {}
    GLuint texID;
    GLenum bindedToUnit;

    virtual void bind();
};

class TextureCubeMap : public Texture
{
public:
    TextureCubeMap(const std::vector<std::string> & pathsToFiles);

private:
    virtual void bind();
};

#endif // TEXTURE_H
