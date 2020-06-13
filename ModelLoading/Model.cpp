//
// Created by BinyBrion on 11/21/2019.
//

#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdexcept>
#include <set>

#include "HelperFunctions.h"

namespace ModelLoading
{
    Model::Model(const std::string &fileLocation)
                :
                    boundingAABB{::World::BoundingVolumes::StaticAABB // Have to default construct an AABB due to no
                                 {                                    // default constructor for ::World::BoundingVolumes::StaticAABB
                                         ::World::XRange{0.0f, 0.0f},
                                         ::World::YRange{0.0f, 0.0f},
                                         ::World::ZRange{0.0f, 0.0f}
                                 }}
    {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(fileLocation.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if( (scene == nullptr) || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            throw std::runtime_error{std::string{"Error reading asset file: "} + importer.GetErrorString()};
        }

        // Recursively find the meshes that make up the model and process them.
        processNode(scene->mRootNode, scene);

        // Find the overall AABB by finding the minimum and maximum points of all the vertices of the meshes that make up the model
        std::vector<glm::vec3> boundingBoxExtremes;

        for(const auto &i : meshes)
        {
            boundingBoxExtremes.insert(boundingBoxExtremes.end(), i.getVertices().begin(), i.getVertices().end());
        }

        BoundingBoxExtremes totalExtremes = findAABB(boundingBoxExtremes);

        boundingAABB =  ::World::BoundingVolumes::StaticAABB
                {
                        ::World::XRange{totalExtremes.minPoint.x, totalExtremes.maxPoint.x},
                        ::World::YRange{totalExtremes.minPoint.y, totalExtremes.maxPoint.y},
                        ::World::ZRange{totalExtremes.minPoint.z, totalExtremes.maxPoint.z}
                };
    }

    const std::vector<::World::BoundingVolumes::StaticAABB>& Model::getBoundingVolumes() const
    {
        return meshBoundingVolumes;
    }

    ::World::BoundingVolumes::StaticAABB Model::getOverallBoundingVolume() const
    {
        return boundingAABB;
    }

    const std::vector<Mesh>& Model::getMeshes() const
    {
        return meshes;
    }

    void Model::processMesh(const aiMesh *const mesh, const aiScene *const scene)
    {
        if(mesh == nullptr || scene == nullptr)
        {
            throw std::runtime_error{"Fatal error processing asset file. Likely error in assimp!: " + std::string{__PRETTY_FUNCTION__}};
        }

        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> indices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textureCoords;

        // Get the mesh'es vertices, normals and texture coordinates
        for(unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            glm::vec3 vertex;

            vertex.x = mesh->mVertices[i].x;
            vertex.y = mesh->mVertices[i].y;
            vertex.z = mesh->mVertices[i].z;

            glm::vec3 normal;

            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;

            if(mesh->mTextureCoords[0] != nullptr)
            {
                glm::vec2 texCoord{0.0f, 0.0f};

                texCoord.x = mesh->mTextureCoords[0][i].x;
                texCoord.y = mesh->mTextureCoords[0][i].y;

                textureCoords.push_back(texCoord);
            }

            vertices.push_back(vertex);
            normals.push_back(normal);
        }

        // Get the mesh's texture coordinates
        for(unsigned int i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];

            for(unsigned int j = 0; j < face.mNumIndices; ++j)
            {
                indices.push_back(face.mIndices[j] + numberVerticesLoaded);
            }
        }

        // Get the texture used by the mesh, if any. Note that a mesh as of right now can only support one texture due
        // to how the rendering is down.
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        assert(material->GetTextureCount(aiTextureType_DIFFUSE) <= 1);

        aiString string;

        if(material->GetTextureCount(aiTextureType_DIFFUSE) == 1)
        {
            material->GetTexture(aiTextureType_DIFFUSE, 0, &string);

            Mesh loadedMesh{vertices, indices, normals, textureCoords, string.C_Str()};

            meshes.push_back(loadedMesh);
        }
        else
        {
            Mesh loadedMesh{vertices, indices, normals, textureCoords, ""};

            meshes.push_back(loadedMesh);
        }

        // Ensure that subsequent meshes' indices refer to its associated mesh vertices.
        numberVerticesLoaded += vertices.size();
    }

    void Model::processNode(const aiNode *const node, const aiScene *const scene)
    {
        if(node == nullptr || scene == nullptr)
        {
            throw std::runtime_error{"Fatal error processing asset file. Likely error in assimp: " + std::string{__PRETTY_FUNCTION__}};
        }

        // Process all of the mesh in the model

        for(unsigned int i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

            if(mesh == nullptr)
            {
                throw std::runtime_error{"Fatal error processing asset file. Likely error in assimp!: " + std::string{__PRETTY_FUNCTION__}};
            }

            processMesh(mesh, scene);
        }

        for(unsigned int i = 0; i < node->mNumChildren; ++i)
        {
            processNode(node->mChildren[i], scene);
        }
    }
}