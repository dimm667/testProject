#include "shaderprogram.h"

ShaderSubProgram::ShaderSubProgram(const std::string &pathToFile, GLenum shaderType) :
    shaderID(0),
    sourceCode("")
{
    std::ifstream fileStream;
    fileStream.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        fileStream.open(pathToFile);

        std::stringstream strStream;
        strStream << fileStream.rdbuf();
        fileStream.close();
        sourceCode = strStream.str();
    }
    catch(std::ifstream::failure e)
    {
        throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }

    shaderID = glCreateShader(shaderType);
    const char * sourceCodeCharString = sourceCode.c_str();
    glShaderSource(shaderID, 1, &sourceCodeCharString, NULL);
    glCompileShader(shaderID);

    // check compile errors
    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::COMPILATION_FAILED: " + pathToFile + "\n" + std::string(infoLog));
    }
}

GLuint ShaderSubProgram::getId() const
{
    return shaderID;
}

ShaderSubProgram::~ShaderSubProgram()
{
    glDeleteShader(shaderID);
}


ShaderProgram::ShaderProgram(const std::vector<ShaderSubProgram> & shadersSubprogramms) :
    programId(0)
{
    programId = glCreateProgram();

    for(auto && shSubPr : shadersSubprogramms)
    {
        attach(shSubPr);
    }
    link();
}

void ShaderProgram::attach(const ShaderSubProgram & subbProgramm)
{
     glAttachShader(programId, subbProgramm.getId());
}

void ShaderProgram::link()
{
    glLinkProgram(programId);

    GLint success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog));
    }
}

void ShaderProgram::use()
{
    glUseProgram(programId);
}

void ShaderProgram::setUniform(const std::string & name, float value)
{
    glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string & name, int value)
{
    glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string &name, const glm::mat4 & value, GLboolean transpose)
{
    glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &value[0][0]);
}

void ShaderProgram::setUniform(const std::string & name, const glm::vec3 & value)
{
     glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}
