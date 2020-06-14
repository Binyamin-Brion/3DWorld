//
// Created by binybrion on 6/13/20.
//

#ifndef GAMEPROTOTYPE_MODELRENDERINFORMATION_H
#define GAMEPROTOTYPE_MODELRENDERINFORMATION_H

#include <string>
#include <vector>
#include "MeshRenderInformation.h"

namespace Render::DataStructures
{
    /**
     * Holds all of the mesh rendering data, which comprises a model and the model texture location together.
     */

    struct ModelRenderingInformation
    {
        std::string modelLocation; // This is intended for use with debugging, though it may never be used.
        std::vector<MeshRenderInformation> meshes;
    };
}

#endif //GAMEPROTOTYPE_MODELRENDERINFORMATION_H
