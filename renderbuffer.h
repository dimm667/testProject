#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H
#include <glad/glad.h>

class RenderBuffer
{
public:
    RenderBuffer(GLsizei width, GLsizei height, GLenum internalFormat)
    {
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
    }

    GLuint getId() const {return rbo;}

private:
    GLuint rbo;
};

#endif // RENDERBUFFER_H
