//
// Created by binybrion on 6/20/20.
//

#ifndef GAMEPROTOTYPE_GRIDSECTIONINSTANCERANGE_H
#define GAMEPROTOTYPE_GRIDSECTIONINSTANCERANGE_H

namespace Render::DataStructures
{
    /**
     * Bundles together the information required to store instances of a model within the Render structure.
     */

    struct GridSectionInstanceRange
    {
        unsigned int gridSectionID;
        unsigned int instanceCount;
    };
}

#endif //GAMEPROTOTYPE_GRIDSECTIONINSTANCERANGE_H
