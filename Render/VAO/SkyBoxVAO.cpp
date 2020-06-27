//
// Created by binybrion on 6/27/20.
//

#include "SkyBoxVAO.h"
#include "../Shaders/SkyBoxShaderProgram.h"

namespace Render::VAO
{
    SkyBoxVAO::SkyBoxVAO(Textures::TextureManager &textureManager)
    {
        bind();

        loadModel(textureManager,
                  {getModelAssetFolder().append("skyBoxCube.obj")
                  });

    }

    void SkyBoxVAO::render(Shaders::SkyBoxShaderProgram &skyBoxShaderProgram)
    {
        glDepthFunc(GL_LEQUAL);

        bind();

        // Draw all of the meshes that make up a model.
        for(const auto &i : modelRenderingInformation)
        {
            for(const auto &mesh : i.meshes)
            {
                skyBoxShaderProgram.uploadUInt("textureSamplerIndex", mesh.textureId.textureID);
                skyBoxShaderProgram.uploadInt("textureSamplerOffset", mesh.textureId.offset);

                void* indiceOffsetPtr = (void*)(mesh.indiceOffset * sizeof(unsigned int));

                glDrawElementsBaseVertex(GL_TRIANGLES, mesh.indicesCount, GL_UNSIGNED_INT, indiceOffsetPtr, mesh.baseVertex);
            }
        }

        glDepthFunc(GL_LESS);
    }
}