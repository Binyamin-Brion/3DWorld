//
// Created by binybrion on 6/28/20.
//

#ifndef GAMEPROTOTYPE_ASSETLOCATIONS_H
#define GAMEPROTOTYPE_ASSETLOCATIONS_H

#include <string>

namespace ProgramInformation
{
    /**
     * Provides functions to retrieve the location of assets used in the program.
     */

    struct AssetLocations
    {
        /**
         * Get the location of the flower model file.
         *
         * @return location of the flower model file on the file system
         */
        static std::string getFlowerAssetLocation();

        /**
         * Get the location of the grass model file
         *
         * @return location of th flower model file on the file system
         */
        static std::string getGrassAssetLocation();

        /**
         * Get the location of the skybox model file.
         *
         * @return location of the skybox model file on the file system
         */
        static std::string getSkyBoxLocation();

        /**
         * Get the location of the surface cube model file.
         *
         * @return location of the surface cube model file on the file system
         */
        static std::string getSurfaceCubeLocation();

        /**
         * Get the location of the tree model file.
         *
         * @return location of the tree model file on the file system
         */
        static std::string getTreeAssetLocation();
    };
}

#endif //GAMEPROTOTYPE_ASSETLOCATIONS_H
