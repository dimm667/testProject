#ifndef TEXTURE_H
#define TEXTURE_H
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>

///
/// \brief The Texture class
///
class Texture
{
public:
    using TexDataPointer = void *;      ///<


    ///
    /// \brief Texture
    /// \param type
    ///
    Texture(GLenum type = GL_TEXTURE_2D);

    ///
    /// \brief ~Texture
    ///
    virtual ~Texture();

    ///
    /// \brief getBindUnit
    /// \return
    ///
    virtual GLenum getBindUnit() const;

    ///
    /// \brief activate
    /// \param textureUint
    ///
    virtual void activate(GLenum textureUint);

protected:
    ///
    /// \brief The TextureParamet struct
    ///
    struct TextureParamet
    {
        GLenum name;        ///<
        GLenum value;       ///<
    };

    ///
    using TextureParameterList = std::vector<TextureParamet>;

//    ///
//    /// \brief getId
//    /// \return
//    ///
//    GLenum getId() {return texID;}

    ///
    /// \brief configure
    /// \param params
    ///
    void configure(const TextureParameterList & params);

    GLuint texID =          GL_INVALID_VALUE;       ///<
    GLenum textureType =    GL_TEXTURE_2D;          ///<
    GLenum bindedToUnit =   GL_TEXTURE0;            ///<
};

///
/// \brief The TextureBuffer class
///
class TextureBuffer : public Texture
{
public:
    ///
    /// \brief TextureBuffer
    /// \param width
    /// \param height
    ///
    TextureBuffer(int width, int height, bool isCube = false);

    virtual ~TextureBuffer() = default;

    void bindBuffer();

    ///
    /// \brief getFboId
    /// \return
    ///
    GLuint getFboId() {return fbo;}

private:
    GLuint fbo;         ///<
    GLuint width;
    GLuint height;
};

///
/// \brief The TextureImage class
///
class TextureImage : public Texture
{
public:
    ///
    /// \brief TextureImage
    /// \param pathToFile
    ///
    TextureImage(const std::string & pathToFile);

    virtual ~TextureImage() = default;
};

///
/// \brief The TextureCubeMap class
///
class TextureCubeMap : public Texture
{
public:
    ///
    /// \brief TextureCubeMap
    /// \param pathsToFiles
    ///
    TextureCubeMap(const std::vector<std::string> & pathsToFiles);

    virtual ~TextureCubeMap() = default;
};

#endif // TEXTURE_H
