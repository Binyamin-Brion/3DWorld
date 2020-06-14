//
// Created by BinyBrion on 11/21/2019.
//

#ifndef MINECRAFT_MODEL_H
#define MINECRAFT_MODEL_H

class aiNode;
class aiScene;
class aiMesh;

#include <string>
#include <vector>

#include "Mesh.h"

namespace ModelLoading
{
    /**
     *  Loads a model from a file, and makes it available by returning the list of meshes that compose the model.
     */

    class Model
    {
        public:

            /**
             * Loads the model contained in the passed in file.
             *
             * @param fileLocation where the model information is held
             */
            explicit Model(const std::string &fileLocation);

            /**
             * Get the bounding AABB that compose the individual meshes compose the model.
             *
             * @return vector of AABB of the meshes that make up the model
             */
            [[nodiscard]] const std::vector<::World::BoundingVolumes::StaticAABB>& getBoundingVolumes() const;

            /**
             * Get the AABB that surrounds all of the meshes. Depending on the location of the meshes this AABB may
             * not be a tight fit.
             *
             * @return AABB that encompasses all of the meshes
             */
            [[nodiscard]] ::World::BoundingVolumes::StaticAABB getOverallBoundingVolume() const;

            /**
             * Get the individual meshes that compose the loaded model.
             *
             * @return vector of the model's meshes
             */
            [[nodiscard]] const std::vector<Mesh>& getMeshes() const;

        private:

            /**
             * Extracts all of the required rendering data for a mesh of the model.
             *
             * @param mesh pointer to the mesh resource
             * @param scene pointer to the scene resource
             */
            void processMesh(const aiMesh* const mesh, const aiScene* const scene);

            /**
             * Extracts all of the required rendering data for a mesh of the model.
             *
             * @param mesh pointer to the mesh resource
             * @param scene pointer to the scene resource
             */
            void processNode(const aiNode* const node, const aiScene* const scene);

            std::vector<Mesh> meshes;
            ::World::BoundingVolumes::StaticAABB boundingAABB;
            std::vector<::World::BoundingVolumes::StaticAABB> meshBoundingVolumes;
    };
}

#endif //MINECRAFT_MODEL_H
