#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class ShaderSubProgram
{
public:
    ShaderSubProgram(const std::string & pathToFile, GLenum shaderType);
    GLuint getId() const;

    ~ShaderSubProgram();

private:
    std::string sourceCode;
    GLuint shaderID;
};


class ShaderProgram
{
public:
    ShaderProgram(const std::vector<ShaderSubProgram> & shadersSubprogramms);

    void use();

    void setUniform(const std::string & name, float value);
    void setUniform(const std::string & name, int value);
    void setUniform(const std::string & name, bool value);
    void setUniform(const std::string & name, const glm::mat4 & value, GLboolean transpose = GL_FALSE);
    void setUniform(const std::string & name, const glm::vec3 & value);

private:
    void attach(const ShaderSubProgram & subbProgramm);
    void link();
    GLuint programId;
};



#endif // SHADERPROGRAM_H
