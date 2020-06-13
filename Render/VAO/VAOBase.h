//
// Created by binybrion on 6/12/20.
//

#ifndef GAMEPROTOTYPE_VAOBASE_H
#define GAMEPROTOTYPE_VAOBASE_H

#include <experimental/filesystem>

namespace Render::VAO
{
    class VAOBase
    {
        public:

            void bind();

        protected:
            VAOBase();
            std::experimental::filesystem::path getModelAssetFolder();

            unsigned vao;
    };
}

#endif //GAMEPROTOTYPE_VAOBASE_H
