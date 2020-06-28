//
// Created by binybrion on 6/28/20.
//

#ifndef GAMEPROTOTYPE_RENDERINFORMATION_H
#define GAMEPROTOTYPE_RENDERINFORMATION_H

#include <string>
#include <vector>
#include <vec3.hpp>
#include "GridSectionInstanceRange.h"

namespace Render::DataStructures
{
    /**
     *  Bundles together the required information to stage (when staging data to appropriate VAO) and render static objects in the world.
     */

    struct RenderInformation
    {
        std::string modelFileName;
        std::vector<glm::vec3> translations;
        std::vector<GridSectionInstanceRange> gridSectionInstanceRange;
    };
}

#endif //GAMEPROTOTYPE_RENDERINFORMATION_H
