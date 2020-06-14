//
// Created by binybrion on 6/8/20.
//

#include "CommandCentre.h"
#include <vec3.hpp>
#include <algorithm>
#include "../Window/Camera/CameraObject.h"
#include "../World/WorldLogic/GridSection.h"
#include "../ProgramInformation/WorldSettings.h"

namespace Render
{
    void CommandCentre::render(const Window::Camera::CameraObject &camera)
    {
        frustumCuller.updatePlaneCoefficients(camera.getProjectionMatrix() * camera.getViewMatrix());

        instanceShaderProgram.useProgram();
        instanceShaderProgram.uploadMat4x4("projection", camera.getProjectionMatrix());
        instanceShaderProgram.uploadMat4x4("view", camera.getViewMatrix());

        const std::vector<unsigned int> visibleGridSections = findVisibleGridSections(camera);

        terrainVao.render(instanceShaderProgram, visibleGridSections);
    }

    void CommandCentre::uploadWorld(const std::vector<std::vector<World::WorldLogic::GridSection>> &gridSections)
    {
        std::vector<glm::vec3> instanceTranslations;

        for(const auto &i : gridSections)
        {
            gridSectionsInformation.emplace_back();

            for(const auto &j : i)
            {
                GridSectionInformation gridSectionInformation{j.getGridSectionID(), j.getSurroundingCube()};

                gridSectionsInformation.back().push_back(gridSectionInformation);

                for(const auto &surfaceCube : j.getSurfaceCubes())
                {
                    instanceTranslations.emplace_back(surfaceCube.getXRange().getMin(), surfaceCube.getYRange().getMin(), surfaceCube.getZRange().getMin());
                }
            }
        }

        terrainVao.uploadInstanceTranslations(instanceTranslations);
    }

    std::vector<unsigned int> CommandCentre::findVisibleGridSections(const Window::Camera::CameraObject &camera) const
    {
        // This bounding box holds all of the world which can be visible from the current camera spot. It is used to reduce
        // the amount of grid sections to check for visibility.
        World::BoundingVolumes::StaticAABB surroundingAABB
                {
                    World::XRange(camera.getPosition().x - camera.getDrawDistance(), camera.getPosition().x + camera.getDrawDistance()),
                    World::YRange(-std::numeric_limits<unsigned int>::max(), std::numeric_limits<unsigned int>::max()),
                    World::ZRange(camera.getPosition().z- camera.getDrawDistance(), camera.getPosition().z + camera.getDrawDistance())
                };

        std::vector<unsigned int> visibleGridSections;

        // Iterate over the AABB of the cubes in the currently generated parts of the world to determine which ones are visible
        // and thus should be rendered.

        // Aliases for required variables for readability purposes.
        unsigned int gridSectionLength = ProgramInformation::WorldSettings::getGridSectionLength();

        unsigned int gridSectionsPerWorldLength = ProgramInformation::WorldSettings::getWorldLength() / gridSectionLength;

        // Min value of lowerX is 0, for when the camera is right along the negative edge of the world.
        // Values to max functions are float but will be truncated when assigning to lowerX.
        unsigned int lowerX = std::max(surroundingAABB.getXRange().getMin() / gridSectionLength, 0.0f);

        // Max value of higherX is the number of grid sections that can fit in one dimension of the world. std::ceil is used
        // to ensure that if a part of a grid section is within the cutoff AABB, it is still included in the visibility check.
        // As with lowerX, arguments to std::min are float, but it does not matter here due to std::ceil.
        unsigned int higherX = std::min(std::ceil(surroundingAABB.getXRange().getMax() / gridSectionLength), static_cast<float>(gridSectionsPerWorldLength));

        // Same idea as with lowerX and higherX respectively.

        unsigned int lowerZ = std::max(surroundingAABB.getZRange().getMin() / gridSectionLength, 0.0f);

        unsigned int higherZ = std::min(std::ceil(surroundingAABB.getZRange().getMax() / gridSectionLength), static_cast<float>(gridSectionsPerWorldLength));

        #pragma omp parallel for
        for(unsigned int x = lowerX; x < higherX; ++x)
        {
            for(unsigned int z = lowerZ; z < higherZ; ++z)
            {
                const World::BoundingVolumes::StaticAABB gridSectionAABB = gridSectionsInformation[x][z].surroundingAABB;

                bool xRangeOverlap = surroundingAABB.getXRange().overlapRange(gridSectionAABB.getXRange());
                bool zRangeOverlap = surroundingAABB.getZRange().overlapRange(gridSectionAABB.getZRange());

                if(!xRangeOverlap && !zRangeOverlap)
                {
                    break;
                }

                glm::vec3 aabbCorners[8];

                aabbCorners[0] = glm::vec3{gridSectionAABB.getXRange().getMin(), gridSectionAABB.getYRange().getMin(), gridSectionAABB.getZRange().getMin()};

                aabbCorners[1] = glm::vec3{gridSectionAABB.getXRange().getMin(), gridSectionAABB.getYRange().getMax(), gridSectionAABB.getZRange().getMin()};

                aabbCorners[2] = glm::vec3{gridSectionAABB.getXRange().getMax(), gridSectionAABB.getYRange().getMax(), gridSectionAABB.getZRange().getMin()};

                aabbCorners[3] = glm::vec3{gridSectionAABB.getXRange().getMax(), gridSectionAABB.getYRange().getMin(), gridSectionAABB.getZRange().getMin()};

                aabbCorners[4] = glm::vec3{gridSectionAABB.getXRange().getMin(), gridSectionAABB.getYRange().getMin(), gridSectionAABB.getZRange().getMax()};

                aabbCorners[5] = glm::vec3{gridSectionAABB.getXRange().getMin(), gridSectionAABB.getYRange().getMax(), gridSectionAABB.getZRange().getMax()};

                aabbCorners[6] = glm::vec3{gridSectionAABB.getXRange().getMax(), gridSectionAABB.getYRange().getMax(), gridSectionAABB.getZRange().getMax()};

                aabbCorners[7] = glm::vec3{gridSectionAABB.getXRange().getMax(), gridSectionAABB.getYRange().getMin(), gridSectionAABB.getZRange().getMax()};

                // AABB that in the grid section very close to the camera may incorrectly be culled. Fix this by automatically including the grid section
                // that the camera is located in.
                if (gridSectionAABB.checkIntersectionPoint(camera.getPosition()))
                {
                    #pragma omp critical
                    {
                        visibleGridSections.push_back(gridSectionsInformation[x][z].gridSectionID);
                    }

                    continue;
                }

                // Check all of the corners of the grid section AABB to see if a corner is in the frustum.

                for (const auto &corner : aabbCorners)
                {
                    if (frustumCuller.pointInFrustum(corner, true))
                    {
                        #pragma omp critical
                        {
                            visibleGridSections.push_back(gridSectionsInformation[x][z].gridSectionID);
                        }

                        break;
                    }
                }
            }
        }

        std::sort(visibleGridSections.begin(), visibleGridSections.end());

        return visibleGridSections;
    }
}