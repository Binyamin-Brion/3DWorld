//
// Created by binybrion on 6/12/20.
//

#include <glad.h>
#include "VAOBase.h"

namespace Render::VAO
{
    void VAOBase::bind()
    {
        glBindVertexArray(vao);
    }

    // Start of protected functions

    VAOBase::VAOBase()
    {
        glGenVertexArrays(1, &vao);
    }

    std::experimental::filesystem::path VAOBase::getModelAssetFolder()
    {
        return std::experimental::filesystem::current_path().parent_path().append("Assets/Models");
    }
}