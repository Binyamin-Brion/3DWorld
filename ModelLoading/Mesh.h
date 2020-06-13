//
// Created by BinyBrion on 11/21/2019.
//

#ifndef MINECRAFT_MESH_H
#define MINECRAFT_MESH_H

#include <vector>
#include <vec2.hpp>
#include <string>
#include "../World/BoundingVolumes/StaticAABB.h"

namespace ModelLoading
{
    /**
     *  Abstracts the pieces of data that are needed to represent a component to render a component within a model.
     *  It is a "container" that aggregates the necessary rendering data for a mesh, and contains an AABB around the
     *  vertices that make this mesh.
     */

    class Mesh
    {
        public:

            /**
             * Initializes the mesh with the necessary information used to for rendering.
             *
             * @param vertices that the mesh is composed of in model space
             * @param indices used to specify the ordering of vertices when rendering
             * @param normals associated with each vertex
             * @param textureCoords that each vertex has
             * @param textureLocation location to the model on the file system from which the mesh data was loaded
             */
            Mesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices,
                 std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, std::string textureLocation);

            /**
             * Get the bounding box of this mesh.
             *
             * @return bounding box that surrounds the extrema of the vertices in all three dimensions
             */
            [[nodiscard]] const ::World::BoundingVolumes::StaticAABB& getBoundingAABB() const;

            /**
             * Get the indices that make up this mesh.
             *
             * @return reference to the indices vector
             */
            [[nodiscard]] const std::vector<unsigned int>& getIndices() const;

            /**
             * Get the normals that are associated with each mesh vertex.
             *
             * @return reference to the normal vector
             */
            [[nodiscard]] const std::vector<glm::vec3>& getNormals() const;

            /**
             * Get the texture coordinates associated with each mesh vertex.
             *
             * @return reference to the texture coordinates vector
             */
            [[nodiscard]] const std::vector<glm::vec2>& getTextureCoords() const;

            /**
             * Get the texture location that the mesh uses.
             *
             * @return the location of the texture on the file system used by the mesh
             */
            [[nodiscard]] const std::string& getTextureLocation() const;

            /**
             * Get the vertices that make up this mesh.
             *
             * @return reference to the vertices vector
             */
            [[nodiscard]] const std::vector<glm::vec3>& getVertices() const;


        private:
            std::vector<glm::vec3> vertices;
            std::vector<unsigned int> indices;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> textureCoords;
            std::string textureLocation;

            ::World::BoundingVolumes::StaticAABB boundingAABB;
    };
}

#endif //MINECRAFT_MESH_H
