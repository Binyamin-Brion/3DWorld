//
// Created by binybrion on 5/18/20.
//

#ifndef VOXEL_L_SYSTEM_COMPRESSFACTOR_H
#define VOXEL_L_SYSTEM_COMPRESSFACTOR_H

namespace Render
{
    namespace Textures
    {
        /**
         * Stores the compression factor required for texture coordinates if an uploaded texture was changed size to
         * meet the available sizes of one of the created texture arrays.
         */

        struct CompressFactor
        {
            float xFactor;
            float yFactor;
        };
    }
}

#endif //VOXEL_L_SYSTEM_COMPRESSFACTOR_H
