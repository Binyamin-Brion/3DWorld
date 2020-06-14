//
// Created by binybrion on 6/12/20.
//

#include "TerrainVAO.h"
#include "../../ModelLoading/Model.h"
#include "../Shaders/InstanceShaderProgram.h"
#include "../../ProgramInformation/WorldSettings.h"

namespace Render::VAO
{
    TerrainVAO::TerrainVAO(Textures::TextureManager &textureManager)
    {
        bind();

        loadModel(textureManager, getModelAssetFolder().append("surfaceCubeGrass.obj").c_str());

        instanceTranslations.bind();
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glVertexAttribDivisor(2, 1);
        glEnableVertexAttribArray(2);
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

            // Iterate through all of the models that make up the terrain and render them.
            for(const auto &modelData : modelRenderingInformation)
            {
                for(const auto &meshData : modelData.meshes)
                {
                    instanceShaderProgram.uploadInt("textureSamplerIndex", meshData.textureId.textureID);
                    instanceShaderProgram.uploadInt("textureSamplerOffset", meshData.textureId.offset);

                    void* indiceOffsetPtr = (void*)(meshData.indiceOffset * sizeof(unsigned int));

                    glDrawElementsInstancedBaseVertexBaseInstance(GL_TRIANGLES, meshData.indicesCount, GL_UNSIGNED_INT, indiceOffsetPtr, elementCount, meshData.baseVertex, elementOffset);
                }
            }
        }
    }

    void TerrainVAO::uploadInstanceTranslations(const std::vector<glm::vec3> &translations)
    {
        instanceTranslations.uploadData(translations);
    }
}