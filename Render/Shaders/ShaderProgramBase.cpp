//
// Created by binybrion on 6/8/20.
//

#include "ShaderProgramBase.h"
#include <stdexcept>
#include <fstream>

namespace Render::Shaders
{
    void ShaderProgramBase::uploadInt(const std::string &uniformName, int value)
    {
        glUniform1i(glGetUniformLocation(program, uniformName.c_str()), value);
    }

    void ShaderProgramBase::uploadUInt(const std::string &uniformName, unsigned int value)
    {
        glUniform1ui(glGetUniformLocation(program, uniformName.c_str()), value);
    }

    void ShaderProgramBase::uploadMat4x4(const std::string &uniformName, const glm::mat4 &matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(program, uniformName.c_str()), 1, GL_FALSE, &matrix[0][0]);
    }

    void ShaderProgramBase::useProgram()
    {
        glUseProgram(program);
    }

    // Beginning of private functions

    void ShaderProgramBase::compileShader(unsigned int &shader, GLenum shaderType, const std::string &sourceLocation)
    {
        shader = glCreateShader(shaderType);

        const std::string sourceCode = readShader(sourceLocation);

        const char *sourceCodeCStr = sourceCode.c_str();

        glShaderSource(shader, 1, &sourceCodeCStr, nullptr);
        glCompileShader(shader);

        checkErrorShader(shader, sourceLocation);
    }

    void ShaderProgramBase::checkErrorShader(unsigned shader, const std::string &sourceLocation)
    {
        int success;

        const unsigned int ERROR_MESSAGE_LENGTH = 1024;
        char errorMessage[ERROR_MESSAGE_LENGTH];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(shader, ERROR_MESSAGE_LENGTH, nullptr, errorMessage);

            glDeleteShader(shader);

            throw std::runtime_error{"Error compiling shader: " + std::string{errorMessage} + " at: " + __PRETTY_FUNCTION__  + " in source file: " + sourceLocation};
        }
    }

    void ShaderProgramBase::checkLinkProgram()
    {
        int success;

        const unsigned int ERROR_MESSAGE_LENGTH = 1024;
        char errorMessage[ERROR_MESSAGE_LENGTH];

        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if(!success)
        {
            glGetProgramInfoLog(program, ERROR_MESSAGE_LENGTH, nullptr, errorMessage);

            glDeleteProgram(program);

            throw std::runtime_error{"Error linking program: " + std::string{errorMessage} + " at: " + __PRETTY_FUNCTION__ };
        }
    }

    std::experimental::filesystem::path ShaderProgramBase::getSourceFolderLocation() const
    {
        return std::experimental::filesystem::current_path().parent_path().append("Render/Shaders/Source");
    }

    void ShaderProgramBase::linkProgram(std::vector<unsigned int> shaders)
    {
        program = glCreateProgram();

        for(unsigned int shader : shaders)
        {
            glAttachShader(program, shader);
        }

        glLinkProgram(program);

        checkLinkProgram();
    }

    std::string ShaderProgramBase::readShader(const std::string &shaderSourceLocation) const
    {
        std::ifstream fileReader{shaderSourceLocation};

        if(!fileReader.is_open())
        {
            throw std::runtime_error{"Unable to open the file: " + shaderSourceLocation +  " at: " + __PRETTY_FUNCTION__};
        }

        std::string currentLine;

        std::string shaderSourceCode;

        while(std::getline(fileReader, currentLine))
        {
            shaderSourceCode += currentLine;

            shaderSourceCode += '\n';
        }

        return shaderSourceCode;
    }
}