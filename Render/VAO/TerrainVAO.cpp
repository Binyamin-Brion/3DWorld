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

        loadModel(textureManager,
                  {getModelAssetFolder().append("surfaceCubeGrass.obj")
                  });

        instanceTranslations.bind();
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glVertexAttribDivisor(2, 1);
        glEnableVertexAttribArray(2);
    }

    void TerrainVAO::render(Shaders::InstanceShaderProgram &instanceShaderProgram, const std::vector<unsigned int> &visibleGridSection)
    {//printf("%lu \n", visibleGridSection.size());
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

            // Iterate through all of the models that make up the terrain and render them.
            for(const auto &model : renderStructure.getSortedModels())
            {
                unsigned int elementOffset = model.gridSectionRenderData[startingGridSectionID].instanceOffset;

                // Find how many instances of the current model to render, given the visible grid sections.
                unsigned int elementCount = 0;

                for(unsigned int i = 0; i < gridSectionCount; ++i)
                {
                    elementCount += model.gridSectionRenderData[startingGridSectionID + i].instanceCount;
                }

                // No instances to render; skip loop iteration to avoid draw calls.
                if(elementCount == 0)
                {
                    continue;
                }

                // Draw all of the meshes that make up a model.
                for(const auto &mesh : model.modelRenderingInformation.meshes)
                {
                    instanceShaderProgram.uploadInt("textureSamplerIndex", mesh.textureId.textureID);
                    instanceShaderProgram.uploadInt("textureSamplerOffset", mesh.textureId.offset);

                    void* indiceOffsetPtr = (void*)(mesh.indiceOffset * sizeof(unsigned int));

                    glDrawElementsInstancedBaseVertexBaseInstance(GL_TRIANGLES, mesh.indicesCount, GL_UNSIGNED_INT, indiceOffsetPtr, elementCount, mesh.baseVertex, elementOffset);
                }
            }
        }
    }

    void TerrainVAO::uploadInstanceTranslations(const std::vector<glm::vec3> &translations, const std::vector<DataStructures::GridSectionInstanceRange> &gridSectionsInformation)
    {
        instanceTranslations.uploadData(translations);

        for(const auto &i : modelRenderingInformation)
        {
            if(i.modelLocation == getModelAssetFolder().append("surfaceCubeGrass.obj").c_str())
            {
                renderStructure.addModel(i, gridSectionsInformation);
            }
        }
    }
}