#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <glad/glad.h>
#include <stdexcept>
#include <vector>
#include "renderbuffer.h"

class FrameBuffer
{
public:
    FrameBuffer(GLenum fbTarget = GL_FRAMEBUFFER) :
        target(fbTarget)
    {
        if(fbTarget != GL_DRAW_FRAMEBUFFER &&
           fbTarget != GL_READ_FRAMEBUFFER &&
           fbTarget != GL_FRAMEBUFFER)
        {
            throw std::runtime_error(std::string("ERROR::FRAMEBUFFER::CREATING\n") + std::string("Invalid frame buffer targed.\n"));
        }

        glGenFramebuffers(1, &fbo);
        if(glGetError() == GL_INVALID_VALUE)
        {
            throw std::runtime_error(std::string("ERROR::FRAMEBUFFER::CREATING\n") + std::string("Can't create framebuffer.\n"));
        }
    }

    struct AttachmentConf
    {
        GLenum attachTo;
        GLuint texture;
    };
    using Attachments = std::vector<AttachmentConf>;

    void attachTextures2D(const Attachments & attachments)
    {
        bind();
        std::vector<GLuint> att;
        for(auto && attachmentConf : attachments)
        {
            glFramebufferTexture2D(target, attachmentConf.attachTo, GL_TEXTURE_2D, attachmentConf.texture, 0);
            att.push_back(attachmentConf.attachTo);
        }

        glDrawBuffers(att.size(), &att.front());
        glReadBuffer(GL_NONE);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::runtime_error("ERROR::TEXTUREBUFFER::FRAMEBUFFER\n");
        }
        unbind();
    }

    void attachRenderBuffer(const RenderBuffer & rb, GLenum attachTo)
    {
        bind();
        glFramebufferRenderbuffer(target, attachTo, GL_RENDERBUFFER, rb.getId());
        unbind();
    }

    void attachTextures(const Attachments & attachments)
    {
        bind();
        std::vector<GLuint> att;
        for(auto && attachmentConf : attachments)
        {
            glFramebufferTexture(target, attachmentConf.attachTo, attachmentConf.texture, 0);
            att.push_back(attachmentConf.attachTo);
        }
        glDrawBuffers(att.size(), &att.front());
        glReadBuffer(GL_NONE);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::runtime_error("ERROR::TEXTUREBUFFER::FRAMEBUFFER\n");
        }
        unbind();
    }

    void bind()
    {
        glBindFramebuffer(target, fbo);
    }

    void unbind()
    {
        glBindFramebuffer(target, 0);
    }

    ~FrameBuffer()
    {
        glDeleteFramebuffers(1, &fbo);
    }

private:
//    std::vector<GLuint> att;
    GLuint fbo;
    GLenum target;
};

#endif // FRAMEBUFFER_H
