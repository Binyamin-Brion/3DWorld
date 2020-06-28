//
// Created by binybrion on 6/28/20.
//

#include "AssetLocations.h"
#include <experimental/filesystem>

namespace ProgramInformation
{
    std::string AssetLocations::getFlowerAssetLocation()
    {
        return std::experimental::filesystem::current_path().parent_path().append("Assets/Models/flower.obj");
    }

    std::string AssetLocations::getSkyBoxLocation()
    {
        return std::experimental::filesystem::current_path().parent_path().append("Assets/Models/skyBoxCube.obj");
    }

    std::string AssetLocations::getSurfaceCubeLocation()
    {
        return std::experimental::filesystem::current_path().parent_path().append("Assets/Models/surfaceCubeGrass.obj");
    }

    std::string AssetLocations::getTreeAssetLocation()
    {
        return std::experimental::filesystem::current_path().parent_path().append("Assets/Models/simpleTree.obj");
    }
}
