//
// Created by binybrion on 6/8/20.
//

#ifndef GAMEPROTOTYPE_COMMANDCENTRE_H
#define GAMEPROTOTYPE_COMMANDCENTRE_H

#include "Shaders/InstanceShaderProgram.h"
#include "Shaders/SkyBoxShaderProgram.h"
#include "VAO/TerrainVAO.h"
#include "../World/BoundingVolumes/StaticAABB.h"
#include "FrustumCulling/FrustumCuller.h"
#include "Textures/TextureManager.h"
#include "VAO/SkyBoxVAO.h"

namespace Window::Camera
{
    class CameraObject;
}

namespace World::WorldLogic
{
    class GridSection;
}

namespace Render
{
    /**
     *  The single place where all of the required OpenGL functionality is centralized in. All OpenGL classes are located
     *  here and the order at which they execute their functionality is controlled by this class.
     */

    class CommandCentre
    {
        public:

            CommandCentre();

            /**
             *  Tells all required OpenGL classes to render their data. This function takes care of setting up OpenGL
             *  state before issuing the rendering calls.
             */
            void render(const Window::Camera::CameraObject &camera);

            /**
             * Uploads the world data to the appropriate VAO for rendering.
             *
             * @param gridSections containing the world data to render
             */
            void uploadWorld(const std::vector<std::vector<World::WorldLogic::GridSection>> &gridSections);

        private:

            /**
             * Find the visible grid sections (ie grid sections that are in the viewing frustum).
             *
             * @param camera being used for rendering
             * @return vector of visible grid sections IDs
             */
            [[nodiscard]] std::vector<unsigned int> findVisibleGridSections(const Window::Camera::CameraObject &camera) const;

            /**
             * Stage the instance data for all of the static objects in the world.
             *
             * @param gridSections containing the world data to render
             */
            void stageStaticObjects(const std::vector<std::vector<World::WorldLogic::GridSection>> &gridSections);

            /**
             * Stage the instance data for all of the surface cubes in the world.
             *
             * @param gridSections containing the world data to render
             */
            void stageSurfaceCubes(const std::vector<std::vector<World::WorldLogic::GridSection>> &gridSections);

            Shaders::InstanceShaderProgram instanceShaderProgram;
            Shaders::SkyBoxShaderProgram skyBoxShaderProgram;

            Textures::TextureManager textureManager;

            VAO::TerrainVAO terrainVao;
            VAO::SkyBoxVAO skyBoxVao;
            FrustumCulling::FrustumCuller frustumCuller;

            // Stripped out information of the passed in grid sections, and to avoid holding a reference to the passed in
            // grid sections (as information stored in the grid section is required even after the call to uploadWorld()
            // is done executing).
            struct GridSectionInformation
            {
                unsigned int gridSectionID;
                World::BoundingVolumes::StaticAABB surroundingAABB;
            };

            std::vector<std::vector<GridSectionInformation>> gridSectionsInformation;
    };
}

#endif //GAMEPROTOTYPE_COMMANDCENTRE_H
