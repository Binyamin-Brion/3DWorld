//
// Created by binybrion on 6/20/20.
//

#ifndef GAMEPROTOTYPE_GRIDSECTIONRENDERDATA_H
#define GAMEPROTOTYPE_GRIDSECTIONRENDERDATA_H

namespace Render::DataStructures
{
    /**
     * Holds the range of within the instance VBO for a model within a particular grid section.
     */

    struct GridSectionRenderData
    {
        unsigned int instanceOffset;
        unsigned int instanceCount;
    };
}

#endif //GAMEPROTOTYPE_GRIDSECTIONRENDERDATA_H
