//
// Created by binybrion on 6/12/20.
//

#include "TerrainVAO.h"
#include "../../ModelLoading/Model.h"
#include "../Shaders/InstanceShaderProgram.h"
#include "../../ProgramInformation/WorldSettings.h"

namespace Render::VAO
{
    TerrainVAO::TerrainVAO()
    {
        ModelLoading::Model terrainCube{getModelAssetFolder().append("modelCube.obj").c_str()};

        bind();

        cubeVertices.bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);

        instanceTranslations.bind();
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glVertexAttribDivisor(1, 1);
        glEnableVertexAttribArray(1);

        // Upload cube render data to appropriate buffers

        std::vector<glm::vec3> modelVertices;
        std::vector<unsigned int> modelIndices;

        for(const auto &i : terrainCube.getMeshes())
        {
            modelVertices.insert(modelVertices.end(), i.getVertices().begin(), i.getVertices().end());

            modelIndices.insert(modelIndices.end(), i.getIndices().begin(), i.getIndices().end());
        }

        cubeVertices.uploadData(modelVertices);
        cubeIndices.uploadData(modelIndices);
    }

    void TerrainVAO::render(Shaders::InstanceShaderProgram &instanceShaderProgram, const std::vector<unsigned int> &visibleGridSection)
    {
        instanceShaderProgram.uploadUInt("surfaceCubesPerGridSection", ProgramInformation::WorldSettings::getSurfaceCubesPerGridSection());
        instanceShaderProgram.uploadUInt("gridSectionsPerWorldLength", ProgramInformation::WorldSettings::getWorldLength() / ProgramInformation::WorldSettings::getGridSectionLength());
        instanceShaderProgram.uploadUInt("gridSectionLength",  ProgramInformation::WorldSettings::getGridSectionLength());
        instanceShaderProgram.uploadUInt("surfaceCubesPerGridSection", ProgramInformation::WorldSettings::getSurfaceCubesPerGridSection());

        bind();

        auto iterator = visibleGridSection.begin();

        // Find all adjacent visible grid sections and render them in a single draw call.
        while(iterator != visibleGridSection.end())
        {
            // The start of the adjacent grid section strip.
            unsigned int startingGridSectionID = *iterator;
            unsigned int gridSectionCount = 1;

            // Count the number of adjacent grid sections.
            while(++iterator != visibleGridSection.end())
            {
                unsigned int previousGridSectionID = *(iterator - 1);

                // Current grid section is not adjacent to the previous one.
                if((*iterator) != (previousGridSectionID + 1))
                {
                    break;
                }

                gridSectionCount += 1;
            }

            instanceShaderProgram.uploadUInt("baseGridSectionID", startingGridSectionID);

            unsigned int elementOffset = startingGridSectionID * ProgramInformation::WorldSettings::getSurfaceCubesPerGridSection();
            unsigned int elementCount = gridSectionCount * ProgramInformation::WorldSettings::getSurfaceCubesPerGridSection();

            // Cubes all always being rendered, so the indice count is always 36.
            glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr, elementCount, elementOffset);
        }
    }

    void TerrainVAO::uploadInstanceTranslations(const std::vector<glm::vec3> &translations)
    {
        instanceTranslations.uploadData(translations);
    }
}