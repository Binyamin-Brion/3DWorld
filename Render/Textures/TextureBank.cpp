//
// Created by binybrion on 5/18/20.
//

#include "TextureBank.h"

namespace Render::Textures
{
    TextureBank::TextureBank()
                    :
                        noTextureID{-1, -1}
    {
        loadedTextures.insert("", noTextureID);
    }

    bool TextureBank::containsTexture(const QString &textureLocation) const
    {
        return loadedTextures.contains(textureLocation);
    }

    TextureID TextureBank::getTextureID(const QString &textureLocation) const
    {
        if(!containsTexture(textureLocation))
        {
            return noTextureID;
        }

        return loadedTextures[textureLocation];
    }

    void TextureBank::insertTexture(const QString &textureLocation, TextureID textureID)
    {
        loadedTextures.insert(textureLocation, textureID);
    }
}
