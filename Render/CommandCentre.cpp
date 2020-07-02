//
// Created by binybrion on 6/8/20.
//

#include "CommandCentre.h"
#include <vec3.hpp>
#include <algorithm>
#include "../Window/Camera/CameraObject.h"
#include "../World/WorldLogic/GridSection.h"
#include "../ProgramInformation/WorldSettings.h"
#include "DataStructures/GridSectionInstanceRange.h"
#include <unordered_map>
#include "DataStructures/RenderInformation.h"
#include "../ProgramInformation/AssetLocations.h"

namespace Render
{
    CommandCentre::CommandCentre()
                    :
                        terrainVao{textureManager},
                        skyBoxVao{textureManager}
    {

    }

    void CommandCentre::render(const Window::Camera::CameraObject &camera)
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        frustumCuller.updatePlaneCoefficients(camera.getProjectionMatrix() * camera.getViewMatrix());

        instanceShaderProgram.useProgram();
        instanceShaderProgram.uploadVec3("cameraPosition", camera.getPosition());
        skyBoxShaderProgram.uploadInt("textures_512x512", 0);
        instanceShaderProgram.uploadMat4x4("projection", camera.getProjectionMatrix());
        instanceShaderProgram.uploadMat4x4("view", camera.getViewMatrix());

        const std::vector<unsigned int> visibleGridSections = findVisibleGridSections(camera);

        terrainVao.render(instanceShaderProgram, visibleGridSections);

        skyBoxShaderProgram.useProgram();
        skyBoxShaderProgram.uploadInt("textures_2048x2048", 1);
        skyBoxShaderProgram.uploadMat4x4("projectionMatrix", camera.getProjectionMatrix());
        skyBoxShaderProgram.uploadMat4x4("viewMatrix", glm::mat4x4{glm::mat3x3{camera.getViewMatrix()}});
        skyBoxVao.render(skyBoxShaderProgram);

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

       // printf("%d \n", std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count());
    }

    void CommandCentre::uploadWorld(const std::vector<std::vector<World::WorldLogic::GridSection>> &gridSections)
    {
        stageSurfaceCubes(gridSections);

        stageStaticObjects(gridSections);

        terrainVao.uploadInstanceTranslations();
    }

    std::vector<unsigned int> CommandCentre::findVisibleGridSections(const Window::Camera::CameraObject &camera) const
    {
        // This bounding box holds all of the world which can be visible from the current camera spot. It is used to reduce
        // the amount of grid sections to check for visibility.
        World::BoundingVolumes::StaticAABB surroundingAABB
                {
                    World::XRange(camera.getPosition().x - camera.getDrawDistance(), camera.getPosition().x + camera.getDrawDistance()),
                    World::YRange(-(float)std::numeric_limits<unsigned int>::max(), std::numeric_limits<unsigned int>::max()),
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

        // Only two threads are used as when adding more thread, the performance becomes more unpredictable (probably due to more synchronisation among
        // critical sections, and the fact that there isn't that much work to do here), which can elads to stuttering, To be honest, even two threads
        // may be too much.
        #pragma omp parallel for default(none) shared(lowerX, higherX, lowerZ, higherZ, surroundingAABB, camera, visibleGridSections) num_threads(2)
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

                glm::vec3 aabbCorners[4];

                aabbCorners[0] = glm::vec3{gridSectionAABB.getXRange().getMin(), gridSectionAABB.getYRange().getMin(), gridSectionAABB.getZRange().getMin()};

                aabbCorners[1] = glm::vec3{gridSectionAABB.getXRange().getMin(), gridSectionAABB.getYRange().getMin(), gridSectionAABB.getZRange().getMax()};

                aabbCorners[2] = glm::vec3{gridSectionAABB.getXRange().getMax(), gridSectionAABB.getYRange().getMin(), gridSectionAABB.getZRange().getMin()};

                aabbCorners[3] = glm::vec3{gridSectionAABB.getXRange().getMax(), gridSectionAABB.getYRange().getMin(), gridSectionAABB.getZRange().getMax()};

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
                    if (frustumCuller.pointInFrustumNoHeight(corner, camera.getPosition(), camera.getFront()))
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

    void CommandCentre::stageStaticObjects(const std::vector<std::vector<World::WorldLogic::GridSection>> &gridSections)
    {

        for(const auto &modelFileName : Objects::StaticObject::getSpecifiedModels())
        {
            std::vector<glm::vec3> instanceTranslations;
            std::vector<DataStructures::GridSectionInstanceRange> gridSectionsInstanceRange;

            for(const auto &column : gridSections)
            {
                for(const auto &j : column)
                {
                    const Objects::StaticObject *staticObject = nullptr;

                    for(const auto &addedModel : j.getStaticObjects())
                    {
                        if(addedModel.getModelFileName() == modelFileName)
                        {
                            staticObject = &addedModel;

                            break;
                        }
                    }

                    if(staticObject == nullptr)
                    {
                        continue;
                    }

                    // Store the instance count for each grid section.
                    gridSectionsInstanceRange.push_back(DataStructures::GridSectionInstanceRange{j.getGridSectionID(), static_cast<unsigned int>(staticObject->getPositions().size())});

                    instanceTranslations.insert(instanceTranslations.end(), staticObject->getPositions().begin(), staticObject->getPositions().end());
                }
            }

            DataStructures::RenderInformation renderInformation;
            renderInformation.modelFileName = modelFileName;
            renderInformation.translations = std::move(instanceTranslations);
            renderInformation.gridSectionInstanceRange = std::move(gridSectionsInstanceRange);

            terrainVao.stageInstanceTranslations(renderInformation);
        }

//        for(unsigned int i = 0; i < 1; ++i)
//        {
//            for(const auto &column : gridSections)
//            {
//                gridSectionsInformation.emplace_back();
//
//                for(const auto &j : column)
//                {
//                    if(j.getStaticObjects().empty())
//                    {
//                        continue;
//                    }
//
//                    // Store the instance count for each grid section.
//                    gridSectionsInstanceRange.push_back(DataStructures::GridSectionInstanceRange{j.getGridSectionID(), static_cast<unsigned int>(j.getStaticObjects()[i].getPositions().size())});
//
//                    instanceTranslations.insert(instanceTranslations.end(), j.getStaticObjects()[i].getPositions().begin(), j.getStaticObjects()[i].getPositions().end());
//                }
//            }
//
//            DataStructures::RenderInformation renderInformation;
//            renderInformation.modelFileName = ProgramInformation::AssetLocations::getTreeAssetLocation();
//            renderInformation.translations = std::move(instanceTranslations);
//            renderInformation.gridSectionInstanceRange = std::move(gridSectionsInstanceRange);
//
//            terrainVao.stageInstanceTranslations(renderInformation);
//        }
    }

    void CommandCentre::stageSurfaceCubes(const std::vector<std::vector<World::WorldLogic::GridSection>> &gridSections)
    {
        std::vector<glm::vec3> instanceTranslations;
        std::vector<DataStructures::GridSectionInstanceRange> gridSectionsInstanceRange;

        for(const auto &i : gridSections)
        {
            gridSectionsInformation.emplace_back();

            for(const auto &j : i)
            {
                GridSectionInformation gridSectionInformation{j.getGridSectionID(), j.getSurroundingCube()};

                gridSectionsInformation.back().push_back(gridSectionInformation);

                // Store the instance count for each grid section.
                gridSectionsInstanceRange.push_back(DataStructures::GridSectionInstanceRange{j.getGridSectionID(), static_cast<unsigned int>(j.getSurfaceCubes().size())});

                // Create translations from the cube's AABB.
                for(const auto &surfaceCube : j.getSurfaceCubes())
                {
                    instanceTranslations.emplace_back(surfaceCube.getXRange().getMin(), surfaceCube.getYRange().getMin(), surfaceCube.getZRange().getMin());
                }
            }
        }

        DataStructures::RenderInformation renderInformation;
        renderInformation.modelFileName = ProgramInformation::AssetLocations::getSurfaceCubeLocation();
        renderInformation.translations = std::move(instanceTranslations);
        renderInformation.gridSectionInstanceRange = std::move(gridSectionsInstanceRange);

        terrainVao.stageInstanceTranslations(renderInformation);
    }
}