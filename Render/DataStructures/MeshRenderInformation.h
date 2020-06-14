//
// Created by binybrion on 6/13/20.
//

#ifndef GAMEPROTOTYPE_MESHRENDERINFORMATION_H
#define GAMEPROTOTYPE_MESHRENDERINFORMATION_H

#include "../Textures/TextureID.h"

namespace Render::DataStructures
{
    /**
     * Stores the required rendering data to render a mesh, which is part of a Model.
     */

    struct MeshRenderInformation
    {
        unsigned int baseVertex; // How many vertices are stored in the VBO before the start of this mesh's vertices
        unsigned int indiceOffset; // Offset in elements into the indices VBO (not in bytes!)
        unsigned int indicesCount;
        Textures::TextureID textureId;
    };
}

#endif //GAMEPROTOTYPE_MESHRENDERINFORMATION_H
