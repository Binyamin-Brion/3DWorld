//
// Created by binybrion on 6/27/20.
//

#ifndef GAMEPROTOTYPE_SKYBOXVAO_H
#define GAMEPROTOTYPE_SKYBOXVAO_H

#include "VAOBase.h"

namespace Render::Shaders
{
    class SkyBoxShaderProgram;
}

namespace Render::VAO
{
    /**
     *  Stores and render skybox data.
     *
     *  The textures used in the skybox are stored in the texture manager; the vertex data for the skybox is stored here.
     */

    class SkyBoxVAO : public VAOBase
    {
        public:

            /**
             * Initializes the VAO by uploading the skybox model needed to render the skybox.
             *
             * @param textureManager that holds all of the textures used in the program
             */
            explicit SkyBoxVAO(Textures::TextureManager &textureManager);

            /**
             * Renders the skybox.
             *
             * The skybox should be rendered last to ensure that everything that is visible is in fact visible!
             *
             * Note: This function will change uniforms in the passed in shader program!
             *
             * @param skyBoxShaderProgram used to render the skybox
             */
            void render(Shaders::SkyBoxShaderProgram &skyBoxShaderProgram);

        private:
    };
}

#endif //GAMEPROTOTYPE_SKYBOXVAO_H
