//
// Created by binybrion on 6/20/20.
//

#ifndef GAMEPROTOTYPE_SORTEDMODEL_H
#define GAMEPROTOTYPE_SORTEDMODEL_H

#include "../Textures/TextureID.h"
#include "GridSectionRenderData.h"
#include "ModelRenderInformation.h"
#include <vector>

namespace Render::DataStructures
{
    /**
     * Stores the information required to render a model as well as the information to retrieve instance data for each
     * grid section in the world.
     */

    struct SortedModel
    {
        ModelRenderingInformation modelRenderingInformation;
        std::vector<GridSectionRenderData> gridSectionRenderData;
    };
}

#endif //GAMEPROTOTYPE_SORTEDMODEL_H
