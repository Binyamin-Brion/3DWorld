//
// Created by binybrion on 6/8/20.
//

#include "CommandCentre.h"
#include <vec3.hpp>

namespace Render
{
    CommandCentre::CommandCentre()
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glm::vec3 vertexData[] =
                {
                    glm::vec3{-0.5f, 0.0f, 0.0f},
                    glm::vec3{0.5f, 0.0f, 0.0f},
                    glm::vec3{0.0f, 0.5f, 0.0f}
                };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);

        instanceShaderProgram.useProgram();
    }

    void CommandCentre::render()
    {
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}