//
// Created by binybrion on 6/12/20.
//

#ifndef GAMEPROTOTYPE_TERRAINVAO_H
#define GAMEPROTOTYPE_TERRAINVAO_H

#include <vec3.hpp>
#include "../VBO/BufferData.h"
#include "VAOBase.h"

namespace Render::Shaders
{
    class InstanceShaderProgram;
}

namespace Render::VAO
{
    class TerrainVAO : public VAOBase
    {
        public:

            TerrainVAO();
            void render(Shaders::InstanceShaderProgram &instanceShaderProgram, const std::vector<unsigned int> &visibleGridSections);
            void uploadInstanceTranslations(const std::vector<glm::vec3> &translations);

        private:

            VBO::BufferData<glm::vec3, GL_ARRAY_BUFFER, GL_STATIC_DRAW> cubeVertices;
            VBO::BufferData<unsigned int, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW> cubeIndices;
            VBO::BufferData<glm::vec3, GL_ARRAY_BUFFER, GL_STATIC_DRAW> instanceTranslations;
    };
}

#endif //GAMEPROTOTYPE_TERRAINVAO_H
