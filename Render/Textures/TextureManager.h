//
// Created by binybrion on 5/18/20.
//

#ifndef VOXEL_L_SYSTEM_TEXTUREMANAGER_H
#define VOXEL_L_SYSTEM_TEXTUREMANAGER_H

#include "TextureBank.h"
#include "CompressFactor.h"
#include <QImage>

namespace Render::Textures
{
    class TextureManager
    {
        public:

            /**
             * initializes different internal data required for when the OpenGL context is available for this class.
             */
            TextureManager();

            /**
             * Get the compression factor created when converting an uploaded texture to one of the supported texture sizes.
             *
             * @return the compression factor created when uploading the most recent uploaded texture (even if the
             *          texture was already uploaded into vRam).
             */
            [[nodiscard]] CompressFactor getCompressFactor() const;

            /**
             * Get the texture bank holding the information about how to access textures in vRam.
             *
             * @return reference to the texture bank
             */
            [[nodiscard]] const TextureBank& getTextureBank() const;

            /**
             * Uploads the texture into vRam.
             *
             * @param textureLocation location of the texture to upload on the file disk
             * @throws runtime_exception if the texture on the file system could not be found
             */
            void uploadTexture(const QString &textureLocation);

        private:

            // Logically bundles the required information to manage a texture array.
            struct TextureRelation
            {
                unsigned int textureVBO;
                int textureID;
                int textureCount = 0;
                int maxNumberTextures;
                int dimension;
            };

            /**
             * Creates an image of the desired size, with the passed in image being a subsection of the created image,
             * starting at the top-left corner.
             *
             * @param image for which to embedded into a larger image
             * @param desiredLength length / width of the image to create
             * @param textureLocation the location of the texture stored in the passed in image on the file system
             * @return an image of the requested size with the passed in image embedded in it
             */
            QImage fillUnusedArea(const QImage &image, int desiredLength);

            /**
             * Initializes a texture array based off of the information passed in.
             *
             * @param textureRelations structure specifying the required properties of the created texture array
             */
            void initializeTextureVBO(TextureRelation &textureRelations);

            /**
             * Uploads the texture stored in the passed in image into vRam.
             *
             * @param textureRelations holds the texture VBO resource into which to upload the texture
             * @param textureLocation location of the texture on the file disk that is being held in the passed in QImage
             * @param image the texture to be loaded, stored in a QImage
             */
            void uploadTexture(TextureRelation &textureRelations, const QString &textureLocation, const QImage &image);

            TextureRelation textureRelations_512x512; // Texture arrays for 512x512 textures
            TextureRelation textureRelations_2048x2048; // Texture arrays for 2048x2048 textures

            TextureBank textureBank;
            CompressFactor compressFactor; // The most recent compress factor for the most recent texture upload attempt.
    };
}

#endif //VOXEL_L_SYSTEM_TEXTUREMANAGER_H
