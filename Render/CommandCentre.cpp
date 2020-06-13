//
// Created by binybrion on 6/8/20.
//

#include "CommandCentre.h"
#include <vec3.hpp>
#include "../Window/Camera/CameraObject.h"
#include "../World/WorldLogic/GridSection.h"
#include <omp.h>

namespace Render
{
    CommandCentre::CommandCentre()
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glm::vec3 vertexData[] =
                {
                    glm::vec3{-0.5f, 0.0f, 0.0f},
                    glm::vec3{0.5f, 0.0f, 0.0f},
                    glm::vec3{0.0f, 0.5f, 0.0f}
                };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);

        instanceShaderProgram.useProgram();
    }

    void CommandCentre::render(const Window::Camera::CameraObject &camera)
    {
        frustumCuller.updatePlaneCoefficients(camera.getProjectionMatrix() * camera.getViewMatrix());

        instanceShaderProgram.uploadMat4x4("projection", camera.getProjectionMatrix());
        instanceShaderProgram.uploadMat4x4("view", camera.getViewMatrix());

        const std::vector<unsigned int> visibleGridSections = findVisibleGridSections(camera);

        terrainVao.render(instanceShaderProgram, visibleGridSections);

//        glDrawArrays(GL_TRIANGLES, 0, 3);
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
                    instanceTranslations.emplace_back(surfaceCube.getXRange().getMin(), surfaceCube.getYRange().getMin(), surfaceCube.getZRange().getMax());
                }
            }
        }

        terrainVao.uploadInstanceTranslations(instanceTranslations);
    }

    std::vector<unsigned int> CommandCentre::findVisibleGridSections(const Window::Camera::CameraObject &camera) const
    {
        World::BoundingVolumes::StaticAABB surroundingAABB
                {
                    World::XRange(camera.getPosition().x - camera.getDrawDistance(), camera.getPosition().x + camera.getDrawDistance()),
                    World::YRange(-std::numeric_limits<unsigned int>::max(), std::numeric_limits<unsigned int>::max()),
                    World::ZRange(camera.getPosition().z- camera.getDrawDistance(), camera.getPosition().z + camera.getDrawDistance())
                };

        std::vector<unsigned int> visibleGridSections;

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        // Iterate over the AABB of the cubes in the currently generated parts of the world to determine which ones are visible
        // and thus should be rendered.
        #pragma omp parallel for default(none) shared(surroundingAABB, camera, visibleGridSections)
        for(unsigned int i = 0; i < gridSectionsInformation.size(); ++i)
        {
            std::vector<unsigned int> localVisibleGridSectionID;

            for(const auto &i : gridSectionsInformation[i])
            {
                bool xRangeOverlap = surroundingAABB.getXRange().overlapRange(i.surroundingAABB.getXRange());
                bool zRangeOverlap = surroundingAABB.getZRange().overlapRange(i.surroundingAABB.getZRange());

                if(!xRangeOverlap && !zRangeOverlap)
                {
                    break;
                }

                glm::vec3 aabbCorners[8];

                aabbCorners[0] = glm::vec3{i.surroundingAABB.getXRange().getMin(), i.surroundingAABB.getYRange().getMin(), i.surroundingAABB.getZRange().getMin()};

                aabbCorners[1] = glm::vec3{i.surroundingAABB.getXRange().getMin(), i.surroundingAABB.getYRange().getMax(), i.surroundingAABB.getZRange().getMin()};

                aabbCorners[2] = glm::vec3{i.surroundingAABB.getXRange().getMax(), i.surroundingAABB.getYRange().getMax(), i.surroundingAABB.getZRange().getMin()};

                aabbCorners[3] = glm::vec3{i.surroundingAABB.getXRange().getMax(), i.surroundingAABB.getYRange().getMin(), i.surroundingAABB.getZRange().getMin()};

                aabbCorners[4] = glm::vec3{i.surroundingAABB.getXRange().getMin(), i.surroundingAABB.getYRange().getMin(),i.surroundingAABB.getZRange().getMax()};

                aabbCorners[5] = glm::vec3{i.surroundingAABB.getXRange().getMin(), i.surroundingAABB.getYRange().getMax(),i.surroundingAABB.getZRange().getMax()};

                aabbCorners[6] = glm::vec3{i.surroundingAABB.getXRange().getMax(), i.surroundingAABB.getYRange().getMax(), i.surroundingAABB.getZRange().getMax()};

                aabbCorners[7] = glm::vec3{i.surroundingAABB.getXRange().getMax(), i.surroundingAABB.getYRange().getMin(), i.surroundingAABB.getZRange().getMax()};

                // AABB that in the grid section very close to the camera may incorrectly be culled. Fix this by automatically including the grid section
                // that the camera is located in.
                if (i.surroundingAABB.checkIntersectionPoint(camera.getPosition()))
                {
                    #pragma omp critical
                    visibleGridSections.push_back(i.gridSectionID);

                    continue;
                }

                for (const auto &corner : aabbCorners)
                {
                    if (frustumCuller.pointInFrustum(corner, true))
                    {
                        #pragma omp critical
                        visibleGridSections.push_back(i.gridSectionID);

                        break;
                    }
                }
            }
        }

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//printf("%d \n", omp_thread_count());
        printf("Time Taken: %d \n", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));

        return visibleGridSections;
    }
}