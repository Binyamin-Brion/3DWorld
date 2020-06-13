//
// Created by BinyBrion on 11/21/2019.
//

#include "Mesh.h"
#include <algorithm>

#include "../World/BoundingVolumes/StaticAABB.h"
#include "HelperFunctions.h"

namespace ModelLoading
{
    Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices,
               std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, std::string textureLocation)
               :
                   vertices{std::move(vertices)},
                   indices{std::move(indices)},
                   normals{std::move(normals)},
                   textureCoords{std::move(textureCoords)},
                   textureLocation{std::move(textureLocation)},
                   boundingAABB{::World::BoundingVolumes::StaticAABB
                                           {
                                                   ::World::XRange{0.0f, 0.0f},
                                                   ::World::YRange{0.0f, 0.0f},
                                                   ::World::ZRange{0.0f, 0.0f}
                                           }
                                }
    {

        // Calculate the AABB for this mesh, where each dimensions of the AABB in any dimension is the minimum and
        // maximum mesh points in that direction
        BoundingBoxExtremes boundingBoxExtremes = findAABB(this->vertices);

        boundingAABB = ::World::BoundingVolumes::StaticAABB
                {
                        ::World::XRange{boundingBoxExtremes.minPoint.x, boundingBoxExtremes.maxPoint.x},
                        ::World::YRange{boundingBoxExtremes.minPoint.y, boundingBoxExtremes.maxPoint.y},
                        ::World::ZRange{boundingBoxExtremes.minPoint.z, boundingBoxExtremes.maxPoint.z}
                };

    }

    const ::World::BoundingVolumes::StaticAABB& Mesh::getBoundingAABB() const
    {
        return boundingAABB;
    }

    const std::vector<unsigned int>& Mesh::getIndices() const
    {
        return indices;
    }

    const std::vector<glm::vec3>& Mesh::getNormals() const
    {
        return normals;
    }

    const std::vector<glm::vec2>& Mesh::getTextureCoords() const
    {
        return textureCoords;
    }

    const std::string& Mesh::getTextureLocation() const
    {
        return textureLocation;
    }

    const std::vector<glm::vec3>& Mesh::getVertices() const
    {
        return vertices;
    }
}