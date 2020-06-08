//
// Created by binybrion on 6/8/20.
//

#include "InstanceShaderProgram.h"

namespace Render::Shaders
{
    InstanceShaderProgram::InstanceShaderProgram()
    {
        unsigned int vertexShader;
        unsigned int fragmentShader;

        std::experimental::filesystem::path sourceFolder = getSourceFolderLocation().append("/InstanceShaders");

        compileShader(vertexShader, GL_VERTEX_SHADER, sourceFolder.string() + "/vertexShader.txt");

        compileShader(fragmentShader, GL_FRAGMENT_SHADER, sourceFolder.string() + "/fragmentShader.txt");

        linkProgram({vertexShader, fragmentShader});

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Note that the above delete calls do not actually delete the shaders until they are no longer referenced
        // by any shader programs. After detaching them will they be deleted.
        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
    }
}