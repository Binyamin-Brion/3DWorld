//
// Created by binybrion on 4/9/20.
//

#ifndef GLFW_GAMEWORLD_H
#define GLFW_GAMEWORLD_H

#include <vec3.hpp>
#include "TerrainRange.h"
#include "GridSection.h"
#include <deque>
#include "NewlyAddedCubes.h"
#include "WorldDirection.h"

namespace World::WorldLogic
    {
        /**
         *  Represents the game world, and manages creating the initial world as well as loading new
         *  parts of the world as needed. Provides the translations of cubes in the world for rendering.
         */

        class GameWorld
        {

            public:

                static const unsigned int GRID_SECTION_LENGTH = 32;
                static const unsigned int WORLD_SECTION_LENGTH = 512;
                static const unsigned int GRID_SECTION_PER_WORLD_SECTION_LENGTH = WORLD_SECTION_LENGTH / GRID_SECTION_LENGTH;
                static const unsigned int VISIBLE_WORLD_LENGTH = 3;


                /**
                 *  Creates the initial game world at the specified position.
                 *
                 *  Note: the position cannot be within one world section length of the edges of the world!
                 *
                 * @param cameraPosition position around which to load the initial world
                 */
                void createInitialWorld(glm::vec3 cameraPosition);

                /**
                 * Get the newly added bounding volumes added to the world.
                 *
                 * @return deques containing the newly added AABB to the world
                 */
                const std::deque<std::deque<NewlyAddedCubes>>& getBoundingVolumes() const;

                /**
                 * Get the range of rows of the currently loaded part of the world.
                 *
                 * @return loaded rows of the current visible world.
                 */
                TerrainRange getLoadedColumns() const;

                /**
                 * Get the range of rows of the currently loaded part of the world.
                 *
                 * @return loaded rows of the current visible world.
                 */
                TerrainRange getLoadedRows() const;

                /**
                 *  Load columns to the left of the currently loaded world.
                 *
                 *  Note: the position of the player cannot be within one world section length of the edges of the world!
                 */
                void loadColumnsLeft();

                /**
                 *  Load columns to the right of the currently loaded world.
                 *
                 *  Note: the position of the player cannot be within one world section length of the edges of the world!
                 */
                void loadColumnsRight();

                /**
                 *  Load rows below the currently loaded world.
                 *
                 *  Note: the position of the player cannot be within one world section length of the edges of the world!
                 */
                void loadRowsDown();

                /**
                 *  Load rows above the currently loaded world.
                 *
                 *  Note: the position of the player cannot be within one world section length of the edges of the world!
                 */
                void loadRowsUp();

                /**
                 *  Notify this object that the newly added cubes' information has been used and can be removed.
                 */
                void resetNewlyAddedAABB();

                /**
                 * Set the amount of columns or rows (each one grid section long) to load when calling any of the
                 * loadXXX() functions.
                 *
                 * @param nextLoadRange range to load when calling a loadXXX() function
                 */
                void setNextLoadRange(unsigned nextLoadRange);

            private:

                /**
                 * Gets the part of the world that needs to be checked for any gaps due to being loaded in the last
                 * world load.
                 *
                 * @param loadedDirection direction that the world was last loaded in
                 * @return ranges describing part of the world to be check for gaps
                 */
                std::pair<TerrainRange, TerrainRange> determineGapFillingRange(WorldDirection loadedDirection);

                /**
                * Get the height of the top-most surface cube at the given location.
                *
                * @param indexCubeX x-index of the individual cube relative to the loaded world
                * @param indexCubeZ z-index of the individual cube relative to the loaded world
                * @return height at th passed in location
                */
                unsigned int getCurrentHeight(unsigned int indexCubeX, unsigned int indexCubeZ) const;

                /**
                 * Fills in gaps in the loaded world.
                 *
                 * @param loadedDirection direction that the world was loaded in
                 */
                void fillWorldGaps(WorldDirection loadedDirection);

                /**
                 * Gets the height of the cube directly to the east of the passed in location. If no such cube exists,
                 * the height of the passed in location is returned.
                 *
                 * @param indexCubeX x-index of the individual cube relative to the loaded world
                 * @param indexCubeZ z-index of the individual cube relative to the loaded world
                 * @return height of the cube directly east of the passed in cube, if it exists
                 */
                unsigned int heightEast(unsigned int indexCubeX, unsigned int indexCubeZ) const;

                /**
                 * Gets the height of the cube directly to north east of the passed in location. If no such cube exists,
                 * the height of the passed in location is returned.
                 *
                 * @param indexCubeX x-index of the individual cube relative to the loaded world
                 * @param indexCubeZ z-index of the individual cube relative to the loaded world
                 * @return height of the cube directly north of the passed in cube, if it exists
                 */
                unsigned int heightNorth(unsigned int indexCubeX, unsigned int indexCubeZ) const;

                /**
                 * Gets the height of the cube directly to the south of the passed in location. If no such cube exists,
                 * the height of the passed in location is returned.
                 *
                 * @param indexCubeX x-index of the individual cube relative to the loaded world
                 * @param indexCubeZ z-index of the individual cube relative to the loaded world
                 * @return height of the cube directly south of the passed in cube, if it exists
                 */
                unsigned int heightSouth(unsigned int indexCubeX, unsigned int indexCubeZ) const;

                /**
                 * Gets the height of the cube directly to the west of the passed in location. If no such cube exists,
                 * the height of the passed in location is returned.
                 *
                 * @param indexCubeX x-index of the individual cube relative to the loaded world
                 * @param indexCubeZ z-index of the individual cube relative to the loaded world
                 * @return height of the cube directly west of the passed in cube, if it exists
                 */
                unsigned int heightWest(unsigned int indexCubeX, unsigned int indexCubeZ) const;

                /**
                 * Loads the columns that are specified in the nextLoadedColumns range.
                 */
                void loadNewColumns();

                /**
                 *  Loads the rows that are specified in the nextLoadedRows range.
                 */
                void loadNewRows();

                std::deque<std::deque<GridSection>> loadedGridSections;
                std::deque<std::deque<NewlyAddedCubes>> newlyAddedCubes;

                // Specifies the range of the game world that is currently loaded.
                TerrainRange loadedColumns;
                TerrainRange loadedRows;

                // Specifies the range of the game world to be load next.
                TerrainRange nextLoadedColumns;
                TerrainRange nextLoadedRows;

                // Specifies how much of the game world to load next.
                unsigned int nextLoadRange;
        };
    }

#endif //GLFW_GAMEWORLD_H
