//
// Created by binybrion on 6/8/20.
//

#ifndef GAMEPROTOTYPE_SHADERPROGRAMBASE_H
#define GAMEPROTOTYPE_SHADERPROGRAMBASE_H

#include <string>
#include <glad.h>
#include <mat4x4.hpp>

#include <experimental/filesystem>

namespace Render::Shaders
{
    /**
     * Represents a shader program for an OpenGL program, and is intended to be inherited by concrete ShaderProgram classes
     * that specify how to construct a shader program. Provides abstractions over the raw openGL calls required to work
     * with shader programs.
     */

    class ShaderProgramBase
    {
        public:

            /**
             * Uploads a signed integer into a uniform in the program.
             *
             * Note: The program must be bounded by calling useProgram() first!
             *
             * @param uniformName name of the uniform to for which the unsigned value is intended
             * @param value the uniform should have
             */
            void uploadInt(const std::string &uniformName, int value);

            /**
             * Uploads an unsigned integer into a uniform in the program.
             *
             * Note: The program must be bounded by calling useProgram() first!
             *
             * @param uniformName name of the uniform to for which the unsigned value is intended
             * @param value the uniform should have
             */
            void uploadUInt(const std::string &uniformName, unsigned int value);

            /**
             * Uploads a mat4x4 integer into a uniform in the program.
             *
             * Note: The program must be bounded by calling useProgram() first!
             *
             * @param uniformName name of the uniform to for which the unsigned value is intended
             * @param value the uniform should have
             */
            void uploadMat4x4(const std::string &uniformName, const glm::mat4 &matrix);

            /**
             * Makes the program the current program.
             */
            void useProgram();

        protected:

            ShaderProgramBase() = default;

            /**
             *  Creates a shader of the specified type and compiles it using the shader source location in the passed
             *  in file location.
             *
             * @param shader variable that will reference the created shader
             * @param shaderType the type of shader to be created
             * @param sourceLocation location of the file containing the shader source
             */
            void compileShader(unsigned int &shader, GLenum shaderType, const std::string &sourceLocation);

            /**
             * Checks for any error in the creation of the passed in shader.
             *
             * Any exception with the error message is thrown if an error occurred.
             *
             * @param shader to check for compilation errors
             */
            void checkErrorShader(unsigned shader);

            /**
             * Checks for any error in the creation of shader program.
             *
             * Any exception with the error message is thrown if an error occurred.
             */
            void checkLinkProgram();

            /**
             * Get the location to the root folder holding all of the shader files.
             *
             * @return path to the root shader folder
             */
            std::experimental::filesystem::path getSourceFolderLocation() const;

            /**
             * Creates a program, and then attaches all of the passed in shaders, ending with the shader being linked.
             * Any errors during linking are checked for and an exception is thrown if there is a message in the info log.
             *
             * @param shaders to attach to the program
             */
            void linkProgram(std::vector<unsigned int> shaders);

            /**
             * Reads the contents of the file passed into this function.
             *
             * @param shaderSourceLocation location of file to read
             * @return contents of the passed in file
             */
            std::string readShader(const std::string &shaderSourceLocation) const;

        protected:

            unsigned int program;
    };
}

#endif //GAMEPROTOTYPE_SHADERPROGRAMBASE_H
