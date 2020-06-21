//
// Created by binybrion on 6/12/20.
//

#include <glad.h>
#include "VAOBase.h"
#include "../../ModelLoading/Model.h"
#include "../Textures/TextureManager.h"

namespace Render::VAO
{
    void VAOBase::bind()
    {
        glBindVertexArray(vao);
    }

    // Start of protected functions

    VAOBase::VAOBase()
    {
        glGenVertexArrays(1, &vao);

        bind();

        vertices.bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);

        textureCoordinates.bind();
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
        glEnableVertexAttribArray(1);
    }

    std::experimental::filesystem::path VAOBase::getModelAssetFolder()
    {
        return std::experimental::filesystem::current_path().parent_path().append("Assets/Models");
    }

    void VAOBase::loadModel(Textures::TextureManager &textureManager, const std::vector<std::string> &modelLocations)
    {
        bind();

        std::vector<glm::vec3> modelVertices;
        std::vector<unsigned int> modelIndices;
        std::vector<glm::vec2> modelTextureCoordinates;

        for(const auto &modelLocation : modelLocations)
        {
            ModelLoading::Model model{modelLocation};

            modelRenderingInformation.push_back(DataStructures::ModelRenderingInformation{modelLocation, std::vector<DataStructures::MeshRenderInformation>{}});

            for(const auto &i : model.getMeshes())
            {
                // Prepare texture information.
                textureManager.uploadTexture(QString::fromStdString(i.getTextureLocation()));

                Textures::TextureID textureID = textureManager.getTextureBank().getTextureID(QString::fromStdString(i.getTextureLocation()));

                // Prepare mesh rendering data.
                DataStructures::MeshRenderInformation meshRenderInformation{verticesLoaded, indicesLoaded, static_cast<unsigned int>(i.getIndices().size()), textureID};

                modelRenderingInformation.back().meshes.push_back(meshRenderInformation);

                // Upload the model information into the appropriate vectors to later upload to the GPU.
                modelVertices.insert(modelVertices.end(), i.getVertices().begin(), i.getVertices().end());

                modelIndices.insert(modelIndices.end(), i.getIndices().begin(), i.getIndices().end());

                auto scaledModelTextureCoordinates = i.getTextureCoords();

                // Account for the fact that the loaded model may not be 512x512 resolution, and so the texture coordinates
                // need to be scaled to account for this fact.
                for(auto &coordinate : scaledModelTextureCoordinates)
                {
                    coordinate.x *= textureManager.getCompressFactor().xFactor;
                    coordinate.y *= textureManager.getCompressFactor().yFactor;
                }

                modelTextureCoordinates.insert(modelTextureCoordinates.end(), scaledModelTextureCoordinates.begin(), scaledModelTextureCoordinates.end());

                // Required for creating mesh rendering information.
                verticesLoaded += i.getVertices().size();
                indicesLoaded += i.getIndices().size();
            }
        }

        vertices.uploadData(modelVertices);
        indices.uploadData(modelIndices);
        textureCoordinates.uploadData(modelTextureCoordinates);
    }
}