#ifndef VIEWPORT_H
#define VIEWPORT_H
#include <glad/glad.h>
#include <stdexcept>

class Viewport
{
public:
    Viewport(GLint vp_x, GLint vp_y, GLsizei vp_width, GLsizei vp_height) :
        x(vp_x), y(vp_y), width(vp_width), height(vp_height)
    {
        if(vp_width < 0 || vp_height < 0)
        {
            throw std::runtime_error(std::string("ERROR::VIEWPORT::CREATING\n") + std::string("Invalid view port parameters.\n"));
        }
        activate();
    }

    inline void activate()
    {
        glViewport(x, y, width, height);
    }

private:
    GLint x;
    GLint y;
    GLsizei width;
    GLsizei height;
};

#endif // VIEWPORT_H
