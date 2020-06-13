//
// Created by binybrion on 6/8/20.
//

#ifndef GAMEPROTOTYPE_COMMANDCENTRE_H
#define GAMEPROTOTYPE_COMMANDCENTRE_H

#include "Shaders/InstanceShaderProgram.h"
#include "VAO/TerrainVAO.h"
#include "../World/BoundingVolumes/StaticAABB.h"
#include "FrustumCulling/FrustumCuller.h"

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

            void uploadWorld(const std::vector<std::vector<World::WorldLogic::GridSection>> &gridSections);

        private:

            [[nodiscard]] std::vector<unsigned int> findVisibleGridSections(const Window::Camera::CameraObject &camera) const;

            Shaders::InstanceShaderProgram instanceShaderProgram;

            unsigned int vao;
            unsigned int vbo;

            VAO::TerrainVAO terrainVao;
            FrustumCulling::FrustumCuller frustumCuller;

            struct GridSectionInformation
            {
                unsigned int gridSectionID;
                World::BoundingVolumes::StaticAABB surroundingAABB;
            };

            std::vector<std::vector<GridSectionInformation>> gridSectionsInformation;
    };
}

#endif //GAMEPROTOTYPE_COMMANDCENTRE_H
