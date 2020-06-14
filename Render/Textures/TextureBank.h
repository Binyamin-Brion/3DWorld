//
// Created by binybrion on 5/18/20.
//

#ifndef VOXEL_L_SYSTEM_TEXTUREBANK_H
#define VOXEL_L_SYSTEM_TEXTUREBANK_H

#include <QHash>
#include "TextureID.h"

namespace Render
{
    namespace Textures
    {
        class TextureBank
        {
            public:
                TextureBank();
                bool containsTexture(const QString &textureLocation) const;
                TextureID getTextureID(const QString &textureLocation) const;
                void insertTexture(const QString &textureLocation, TextureID textureID);

            private:
                QHash<QString, TextureID> loadedTextures;
                TextureID noTextureID;
        };
    }
}

#endif //VOXEL_L_SYSTEM_TEXTUREBANK_H
