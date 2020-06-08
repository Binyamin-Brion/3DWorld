//
// Created by binybrion on 4/9/20.
//

#include <cstdio>
#include "GameWorld.h"

namespace World
{
    namespace WorldLogic
    {
        /*
         * The game world uses the following conventions:
         *  - Z (rendered as if -Z, but coordinates are specified with positive Z.)
         *  ^
         *  |    ^ Up
         *  |    |____> Right
         *  |
         *  |-----------> X
         */

        void GameWorld::createInitialWorld(glm::vec3 cameraPosition)
        {
            // Ensures that the previous information about the world, if any, does not affect this instance of the world
            // being loaded.
            loadedGridSections.clear();

            newlyAddedCubes.clear();

           /*
            *  Example calculation: Suppose camera position is at x = 1024.
            *
            *  Then, the beginning of the loadedColumns would be (1024 / WORLD_SECTION_LENGTH) * GRID_SECTION_PER_WORLD_SECTION_LENGTH,
            *  which is 2 * GRID_SECTION_PER_WORLD_SECTION_LENGTH, which at the time of writing would be 32.
            *
            *  The end of the column range is the number of columns in the visible world offset from the beginning loaded columns.
            *  In this case, there are GRID_SECTION_PER_WORLD_SECTION_LENGTH * 3, which at time of this writing is 48.
            *  Thus the end of the loaded columns is the beginning of loaded columns, 32, plus the offset, 48: 32 + 48 = 80.
            */
            nextLoadedColumns.begin = (cameraPosition.x / WORLD_SECTION_LENGTH);

            // Need to load the world section to the left of the current world section that the player is currently in.
            // Hence why the player position cannot be in the first world section.
            nextLoadedColumns.begin -= 1;

            nextLoadedColumns.begin *= GRID_SECTION_PER_WORLD_SECTION_LENGTH;

            nextLoadedColumns.end =   GRID_SECTION_PER_WORLD_SECTION_LENGTH * VISIBLE_WORLD_LENGTH + nextLoadedColumns.begin;

            nextLoadedRows.begin = (cameraPosition.z / WORLD_SECTION_LENGTH);

            // Same logic as the column world section.
            nextLoadedRows.begin -= 1;

            nextLoadedRows.begin *= GRID_SECTION_PER_WORLD_SECTION_LENGTH;

            nextLoadedRows.end =   GRID_SECTION_PER_WORLD_SECTION_LENGTH * VISIBLE_WORLD_LENGTH + nextLoadedRows.begin;

            // The loaded ranges are what the next loaded ranges are, since the creation of a game world calls loadNewColumns()
            // but that function already expects a valid loaded game range (as it is meant to also be called when loaded new
            // parts of the world).
            loadedColumns = nextLoadedColumns;
            loadedRows = nextLoadedRows;

            // Default deques of grid sections are required in order for loadNewColumns() to work. These will be replaced
            // with other deques containing valid data.
            for(unsigned int x = nextLoadedColumns.begin; x < nextLoadedColumns.end; ++x)
            {
                loadedGridSections.emplace_back();

                newlyAddedCubes.emplace_back();
            }

            loadNewColumns();

            // The loadColumns() function will modify the value of the loadedColumns range, but since this is a special
            // instance calling that function, that change makes no sense. As a result, the loadedColumns is updated again here.
            loadedColumns = nextLoadedColumns;
        }

        const std::deque<std::deque<NewlyAddedCubes>>& GameWorld::getBoundingVolumes() const
        {
            return newlyAddedCubes;
        }

        TerrainRange GameWorld::getLoadedColumns() const
        {
            return loadedColumns;
        }

        TerrainRange GameWorld::getLoadedRows() const
        {
            return loadedRows;
        }

        // For all of these loadXXX() functions, remember that a range is [begin, end)! Additionally, the begin variable
        // is smaller than the end variable.

        void GameWorld::loadColumnsLeft()
        {
            nextLoadedColumns.end = loadedColumns.begin;
            nextLoadedColumns.begin = nextLoadedColumns.end - nextLoadRange;

            loadNewColumns();
        }

        void GameWorld::loadColumnsRight()
        {
            nextLoadedColumns.begin = loadedColumns.end;
            nextLoadedColumns.end = nextLoadedColumns.begin + nextLoadRange;

            loadNewColumns();
        }

        void GameWorld::loadRowsDown()
        {
            nextLoadedRows.end = loadedRows.begin;
            nextLoadedRows.begin = nextLoadedRows.end - nextLoadRange;

            loadNewRows();
        }

        void GameWorld::loadRowsUp()
        {
            nextLoadedRows.begin = loadedRows.end;
            nextLoadedRows.end = nextLoadedRows.begin + nextLoadRange;

            loadNewRows();
        }

        void GameWorld::resetNewlyAddedAABB()
        {
            for(auto &i : newlyAddedCubes)
            {
                i.clear();
            }
        }

        void GameWorld::setNextLoadRange(unsigned nextLoadRange)
        {
            this->nextLoadRange = nextLoadRange;
        }

        // Beginning of private functions

        std::pair<TerrainRange, TerrainRange> GameWorld::determineGapFillingRange(WorldDirection loadedDirection)
        {
            static bool firstTimeCalled = true;

            // The first time this function is called is when the initial world is loaded. This is a special case
            // as the entire world has been loaded in the past world load, so the entire world has to be checked for gaps.
            if(firstTimeCalled)
            {
                firstTimeCalled = false;

                return {loadedColumns, loadedRows};
            }

            unsigned int beginX = 0;
            unsigned int endX = 0;

            unsigned int beginZ = 0;
            unsigned int endZ = 0;

            unsigned int numberColumns = loadedColumns.end - loadedColumns.begin;
            unsigned int numberRows = loadedRows.end - loadedRows.begin;

            switch(loadedDirection)
            {
                case WorldDirection::East:

                    // Last 'nextLoadRange' columns in loadedGridSections; all rows.
                    beginX = numberColumns - nextLoadRange;
                    endX = loadedGridSections.size();

                    beginZ = 0;
                    endZ = numberRows;
                    break;

                case WorldDirection::West:

                    // First 'nextLoadRange' columns in loadedGridSections; all rows.
                    beginX = 0;
                    endX = nextLoadRange;

                    beginZ = 0;
                    endZ = numberRows;
                    break;

                case WorldDirection::North:

                    // Last 'nextLoadRange' rows in loadedGridSections; all columns.
                    beginX = 0;
                    endX = numberColumns;

                    beginZ = numberRows - nextLoadRange;
                    endZ = numberColumns;
                    break;

                case WorldDirection::South:

                    // First 'nextLoadRange' rows in loadedGridSections; all columns.
                    beginX = 0;
                    endX = numberColumns;

                    beginZ = 0;
                    endZ = nextLoadRange;
                    break;
            }

            return { {beginX, endX}, {beginZ, endZ} };
        }

        unsigned int GameWorld::getCurrentHeight(unsigned int indexCubeX, unsigned int indexCubeZ) const
        {
            unsigned int columnIndex = indexCubeX / GRID_SECTION_LENGTH;
            unsigned int rowIndex = indexCubeZ / GRID_SECTION_LENGTH;

            // The below indexes are relative to the grid section that the passed in cube is in.
            unsigned int relativeXIndex = indexCubeX - columnIndex * GRID_SECTION_LENGTH;
            unsigned int relativeZIndex = indexCubeZ - rowIndex * GRID_SECTION_LENGTH;

            return loadedGridSections[columnIndex][rowIndex].getHeightAt(relativeXIndex, relativeZIndex);
        }

        void GameWorld::fillWorldGaps(WorldDirection loadedDirection)
        {
           std::pair<TerrainRange, TerrainRange> gapFillingRange = determineGapFillingRange(loadedDirection);

           /*
            * Iterate over every single cube in the part of the world to be checked for gaps, and do the following:
            *
            * > Get the height of the cubes to the north, south, east and west, if they exist
            * > Determine the minimum height of the above four heights
            * > If the height of the iterated over cube is more than two game units above the minimum adjacent height,
            *   keep adding cubes to the iterated cube until this height disparity no longer exists
            */
           for(unsigned int x = gapFillingRange.first.begin; x < gapFillingRange.first.end; ++x)
           {
               for(unsigned int z = gapFillingRange.second.begin; z < gapFillingRange.second.end; ++z)
               {
                   for(unsigned int xIndex = 0; xIndex < GRID_SECTION_LENGTH; ++xIndex)
                   {
                       for(unsigned int zIndex = 0; zIndex < GRID_SECTION_LENGTH; ++zIndex)
                       {
                           unsigned int xPosition = x * GRID_SECTION_LENGTH + xIndex;
                           unsigned int zPosition = z * GRID_SECTION_LENGTH + zIndex;

                           // Minimum adjacent cube height of the iterated over cube.
                           unsigned int localMinHeight = std::min(
                                                                       std::min(
                                                                                 std::min(heightEast(xPosition, zPosition), heightNorth(xPosition, zPosition)),
                                                                                 heightWest(xPosition, zPosition)
                                                                                ),

                                                                       heightSouth(xPosition, zPosition
                                                                   )
                           );

                           unsigned int currentHeight = getCurrentHeight(xPosition, zPosition);

                           unsigned int numberCubesToAdd = currentHeight == 0 ? 0 : // If the cube is already at the lowest possible height, then there is never a need to add filler cubes
                                                           localMinHeight > currentHeight ? 0 : // If the surrounding cubes are higher than the current cube, no cubes need to be added
                                                            (currentHeight - 1) - localMinHeight;

                           // Keep adding cubes underneath the current iterated cube until the height disparity between it
                           // and the lowest adjacent cube disappears.
                           for(unsigned int i = 0; i < numberCubesToAdd; ++i)
                           {
                               // Update the height for the next filler cube.
                               currentHeight -= 1;

                               BoundingVolumes::StaticAABB surfaceCubeAABB
                                       {
                                               XRange(xPosition, xPosition + 1),
                                               YRange(currentHeight, currentHeight + 1),
                                               ZRange(zPosition, zPosition + 1)
                                       };

                               loadedGridSections[x][z].addSurfaceCube(surfaceCubeAABB);
                           }
                       }
                   }
               }
           }
        }

        unsigned int GameWorld::heightEast(unsigned int indexCubeX, unsigned int indexCubeZ) const
        {
            bool lookGridSectionEast = indexCubeX % GRID_SECTION_LENGTH == GRID_SECTION_LENGTH - 1;

            // Grid section Index
            unsigned int columnIndex = indexCubeX / GRID_SECTION_LENGTH;
            unsigned int rowIndex = indexCubeZ / GRID_SECTION_LENGTH;

            // Index within grid section
            unsigned int relativeXIndex = indexCubeX - columnIndex * GRID_SECTION_LENGTH;
            unsigned int relativeZIndex = indexCubeZ - rowIndex * GRID_SECTION_LENGTH;

            // There is no cube to the east; the '- 1' as indexes start at 0.
            if( indexCubeX == (WORLD_SECTION_LENGTH * VISIBLE_WORLD_LENGTH - 1))
            {
                return getCurrentHeight(indexCubeX, indexCubeZ);
            }

            // Get the cube in the same row in the grid section to the east.
            if(lookGridSectionEast)
            {
                return loadedGridSections[columnIndex + 1][rowIndex].getHeightAt(0, relativeZIndex);
            }

            // Get the cube in the same row in the next column of the current grid section.
            return loadedGridSections[columnIndex][rowIndex].getHeightAt(relativeXIndex + 1, relativeZIndex);
        }

        unsigned int GameWorld::heightNorth(unsigned int indexCubeX, unsigned int indexCubeZ) const
        {
            bool lookGridSectionNorth = indexCubeZ % GRID_SECTION_LENGTH == GRID_SECTION_LENGTH - 1;

            // Grid section Index
            unsigned int columnIndex = indexCubeX / GRID_SECTION_LENGTH;
            unsigned int rowIndex = indexCubeZ / GRID_SECTION_LENGTH;

            // Index within grid section
            unsigned int relativeXIndex = indexCubeX - columnIndex * GRID_SECTION_LENGTH;
            unsigned int relativeZIndex = indexCubeZ - rowIndex * GRID_SECTION_LENGTH;

            // There is no cube to the north; the '- 1' as indexes start at 0.
            if( indexCubeZ == (WORLD_SECTION_LENGTH * VISIBLE_WORLD_LENGTH - 1))
            {
                return getCurrentHeight(indexCubeX, indexCubeZ);
            }

            // Get the cube in the same column in the grid section to the north.
            if(lookGridSectionNorth)
            {
                return loadedGridSections[columnIndex][rowIndex + 1].getHeightAt(relativeXIndex, 0);
            }

            // Get the cube in the same column in the next row of the current grid section.
            return loadedGridSections[columnIndex][rowIndex].getHeightAt(relativeXIndex, relativeZIndex + 1);
        }

        unsigned int GameWorld::heightSouth(unsigned int indexCubeX, unsigned int indexCubeZ) const
        {

            bool lookGridSectionSouth = indexCubeZ % GRID_SECTION_LENGTH == 0;

            // Grid section Index
            unsigned int columnIndex = indexCubeX / GRID_SECTION_LENGTH;
            unsigned int rowIndex = indexCubeZ / GRID_SECTION_LENGTH;

            // Index within grid section
            unsigned int relativeXIndex = indexCubeX - columnIndex * GRID_SECTION_LENGTH;
            unsigned int relativeZIndex = indexCubeZ - rowIndex * GRID_SECTION_LENGTH;

            // There is no cube to the south; indexes start at 0.
            if(indexCubeZ == 0)
            {
                return getCurrentHeight(indexCubeX, indexCubeZ);
            }

            // Get the cube in the same column in the grid section to the south.
            if(lookGridSectionSouth)
            {
                return loadedGridSections[columnIndex][rowIndex - 1].getHeightAt(relativeXIndex, 0);
            }

            // Get the cube in the same column in the previous row of the current grid section.
            return loadedGridSections[columnIndex][rowIndex].getHeightAt(relativeXIndex, relativeZIndex - 1);
        }

        unsigned int GameWorld::heightWest(unsigned int indexCubeX, unsigned int indexCubeZ) const
        {

            bool lookGridSectionWest = indexCubeX % GRID_SECTION_LENGTH == 0;

            // Grid section Index
            unsigned int columnIndex = indexCubeX / GRID_SECTION_LENGTH;
            unsigned int rowIndex = indexCubeZ / GRID_SECTION_LENGTH;

            // Index within grid section
            unsigned int relativeXIndex = indexCubeX - columnIndex * GRID_SECTION_LENGTH;
            unsigned int relativeZIndex = indexCubeZ - rowIndex * GRID_SECTION_LENGTH;

            // There is no cube to the west; indexes start at 0.
            if(indexCubeX == 0)
            {
                return getCurrentHeight(indexCubeX, indexCubeZ);
            }

            // Get the cube in the same row in the grid section to the west.
            if(lookGridSectionWest)
            {
                return loadedGridSections[columnIndex - 1][rowIndex].getHeightAt(0, relativeZIndex);
            }

            // Get the cube in the same row in the next column of the previous grid section.
            return loadedGridSections[columnIndex][rowIndex].getHeightAt(relativeXIndex - 1, relativeZIndex);
        }

        void GameWorld::loadNewColumns()
        {
            bool addColumnsLeft = nextLoadedColumns.begin < loadedColumns.begin;

            // This is required in case columns are being added to the left.
            unsigned int iterationCount = 1;

            for(unsigned int x = nextLoadedColumns.begin; x < nextLoadedColumns.end; ++x, ++iterationCount)
            {
                if(addColumnsLeft)
                {
                    loadedGridSections.pop_back();
                    loadedGridSections.emplace_front();

                    newlyAddedCubes.pop_back();
                    newlyAddedCubes.emplace_front();
                }
                else
                {
                    loadedGridSections.pop_front();
                    loadedGridSections.emplace_back();

                    newlyAddedCubes.pop_front();
                    newlyAddedCubes.emplace_back();
                }

                for(unsigned int z = loadedRows.begin; z < loadedRows.end; ++z)
                {
                    if(addColumnsLeft)
                    {
                        // When adding columns to the left, deques are added to the front, but the range starts at the
                        // begin variable. There is a mismatch in that the last deque to be added to the front contains
                        // the xPosition that should be added right to the left of the game world before this function is called.
                        // As a result, when using the range to be loaded, the begin and end range have to be effectively "reversed".

                        unsigned int relativeX = nextLoadedColumns.end - iterationCount;

                        BoundingVolumes::StaticAABB gridSectionAABB
                        {
                            XRange(relativeX * GRID_SECTION_LENGTH, (relativeX + 1) * GRID_SECTION_LENGTH),
                            YRange(std::numeric_limits<float>::min(), std::numeric_limits<float>::max()),
                            ZRange(z * GRID_SECTION_LENGTH, (z + 1) * GRID_SECTION_LENGTH)
                        };

                        loadedGridSections.front().emplace_back(gridSectionAABB);

                        newlyAddedCubes.front().push_back({gridSectionAABB, std::vector<BoundingVolumes::StaticAABB>{}});
                    }
                    else
                    {
                        BoundingVolumes::StaticAABB gridSectionAABB
                        {
                            XRange(x * GRID_SECTION_LENGTH, (x + 1) * GRID_SECTION_LENGTH),
                            YRange(std::numeric_limits<float>::min(), std::numeric_limits<float>::max()),
                            ZRange(z * GRID_SECTION_LENGTH, (z + 1) * GRID_SECTION_LENGTH)
                        };

                        loadedGridSections.back().emplace_back(gridSectionAABB);

                        newlyAddedCubes.back().push_back({gridSectionAABB, std::vector<BoundingVolumes::StaticAABB>{}});
                    }

                    // Add the individual surface cubes to the new grid section
                    for(unsigned int xIndex = 0; xIndex < GRID_SECTION_LENGTH; ++xIndex)
                    {
                        for(unsigned int zIndex = 0; zIndex < GRID_SECTION_LENGTH; ++zIndex)
                        {
                            unsigned int xPosition = x * GRID_SECTION_LENGTH + xIndex;
                            unsigned int zPosition = z * GRID_SECTION_LENGTH + zIndex;

                            unsigned int cubeHeight = 0; // TODO: get height based off of perlin noise

                            if(addColumnsLeft)
                            {
                                // Same reasoning as when adding grid section AABB.

                                unsigned int relativeX = nextLoadedColumns.end - iterationCount;

                                xPosition = relativeX * GRID_SECTION_LENGTH + xIndex;

                                BoundingVolumes::StaticAABB surfaceCubeAABB
                                {
                                    XRange(xPosition, xPosition + 1),
                                    YRange(cubeHeight, cubeHeight + 1), // TODO: Change height
                                    ZRange(zPosition, zPosition + 1)
                                };

                                loadedGridSections.front().back().addSurfaceCube(surfaceCubeAABB);

                                newlyAddedCubes.front().back().newCubes.push_back(surfaceCubeAABB);

                                loadedGridSections.front().back().setHeight(xIndex, zIndex, cubeHeight);
                            }
                            else
                            {
                                BoundingVolumes::StaticAABB surfaceCubeAABB
                                {
                                    XRange(xPosition, xPosition + 1),
                                    YRange(cubeHeight, cubeHeight + 1), // TODO: Change height
                                    ZRange(zPosition, zPosition + 1)
                                };

                                loadedGridSections.back().back().addSurfaceCube(surfaceCubeAABB);

                                newlyAddedCubes.back().back().newCubes.push_back(surfaceCubeAABB);

                                loadedGridSections.back().back().setHeight(xIndex, zIndex, cubeHeight);
                            }
                        }
                    }
                }
            }

            unsigned int columnDifference = nextLoadedColumns.end - nextLoadedColumns.begin;

            if(addColumnsLeft)
            {
                loadedColumns.begin -= columnDifference;
                loadedColumns.end -= columnDifference;
            }
            else
            {
                // The first time loadColumns() is called is a special case as the entire world is loaded, and the
                // loadedColumns variable is updated correctly in the createInitialWorld function, which calls this
                // function. Thus on this first call, the loadedColumns variable should not be updated.

                static bool firstTimeCalled = true;

                if(!firstTimeCalled)
                {
                    loadedColumns.begin += columnDifference;
                    loadedColumns.end += columnDifference;
                }

                firstTimeCalled = false;
            }

            WorldDirection loadedDirection = addColumnsLeft ? WorldDirection::West : WorldDirection::East;

            fillWorldGaps(loadedDirection);
        }

        void GameWorld::loadNewRows()
        {
            bool addRowsDown = nextLoadedRows.begin < loadedRows.begin;

            for(unsigned int x = loadedColumns.begin; x < loadedColumns.end; ++x)
            {
                // Same reasoning as when adding columns to the left of the loaded game world.
                unsigned int iterationCount = 1;

                for(unsigned int z = nextLoadedRows.begin; z < nextLoadedRows.end; ++z, ++iterationCount)
                {
                    unsigned int gridColumnIndex = x - loadedColumns.begin;

                    if(addRowsDown)
                    {
                        // Same reasoning as when adding columns to the left of the loaded game world.
                        unsigned int relativeZ = nextLoadedRows.end - iterationCount;

                        BoundingVolumes::StaticAABB gridSectionAABB
                        {
                            XRange(x * GRID_SECTION_LENGTH, (x + 1) * GRID_SECTION_LENGTH),
                            YRange(std::numeric_limits<float>::min(), std::numeric_limits<float>::max()),
                            ZRange(relativeZ * GRID_SECTION_LENGTH, (relativeZ + 1) * GRID_SECTION_LENGTH)
                        };

                        loadedGridSections[gridColumnIndex].pop_back();
                        loadedGridSections[gridColumnIndex].emplace_front(gridSectionAABB);

                        newlyAddedCubes[gridColumnIndex].push_front({gridSectionAABB, std::vector<BoundingVolumes::StaticAABB>{}});
                    }
                    else
                    {
                        BoundingVolumes::StaticAABB gridSectionAABB
                        {
                            XRange(x * GRID_SECTION_LENGTH, (x + 1) * GRID_SECTION_LENGTH),
                            YRange(std::numeric_limits<float>::min(), std::numeric_limits<float>::max()),
                            ZRange(z * GRID_SECTION_LENGTH, (z + 1) * GRID_SECTION_LENGTH)
                        };

                        loadedGridSections[gridColumnIndex].pop_front();
                        loadedGridSections[gridColumnIndex].emplace_back(gridSectionAABB);

                        newlyAddedCubes[gridColumnIndex].push_back({gridSectionAABB, std::vector<BoundingVolumes::StaticAABB>{}});
                    }

                    // Add the individual surface cubes to the new grid section
                    for(unsigned int xIndex = 0; xIndex < GRID_SECTION_LENGTH; ++xIndex)
                    {
                        for(unsigned int zIndex = 0; zIndex < GRID_SECTION_LENGTH; ++zIndex)
                        {
                            unsigned int xPosition = x * GRID_SECTION_LENGTH + xIndex;
                            unsigned int zPosition = z * GRID_SECTION_LENGTH + zIndex;

                            unsigned int cubeHeight = 0; // TODO: get height based off of perlin noise

                            if(addRowsDown)
                            {
                                // Same reasoning as when adding columns to the left of the loaded game world.
                                unsigned int relativeZ = nextLoadedRows.end - iterationCount;

                                zPosition = relativeZ * GRID_SECTION_LENGTH + zIndex;

                                BoundingVolumes::StaticAABB surfaceCubeAABB
                                {
                                    XRange(xPosition, xPosition + 1),
                                    YRange(cubeHeight, cubeHeight + 1), // TODO: Change height
                                    ZRange(zPosition, zPosition + 1)
                                };

                                loadedGridSections[gridColumnIndex].front().addSurfaceCube(surfaceCubeAABB);

                                newlyAddedCubes[gridColumnIndex].front().newCubes.push_back(surfaceCubeAABB);

                                loadedGridSections[gridColumnIndex].front().setHeight(xIndex, zIndex, cubeHeight);
                            }
                            else
                            {
                                BoundingVolumes::StaticAABB surfaceCubeAABB
                                {
                                    XRange(xPosition, xPosition + 1),
                                    YRange(cubeHeight, cubeHeight + 1), // TODO: Change height
                                    ZRange(zPosition, zPosition + 1)
                                };

                                loadedGridSections[gridColumnIndex].back().addSurfaceCube(surfaceCubeAABB);

                                newlyAddedCubes[gridColumnIndex].back().newCubes.push_back(surfaceCubeAABB);

                                loadedGridSections[gridColumnIndex].back().setHeight(xIndex, zIndex, cubeHeight);
                            }
                        }
                    }
                }
            }

            unsigned int rowDifference = nextLoadedRows.end - nextLoadedRows.begin;

            if(addRowsDown)
            {
                loadedRows.begin -= rowDifference;
                loadedRows.end -= rowDifference;
            }
            else
            {
                loadedRows.begin += rowDifference;
                loadedRows.end += rowDifference;
            }

            WorldDirection loadedDirection = addRowsDown ? WorldDirection::South : WorldDirection::North;

            fillWorldGaps(loadedDirection);
        }
    }
}