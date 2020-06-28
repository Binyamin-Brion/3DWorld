//
// Created by binybrion on 6/28/20.
//

#include <algorithm>
#include "StaticObject.h"

namespace Objects
{
    std::vector<std::string> StaticObject::specifiedModels;

    StaticObject::StaticObject(std::string modelFileName)
                    :
                        modelFileName{std::move(modelFileName)}
    {
        if(std::find(specifiedModels.begin(), specifiedModels.end(), modelFileName) == specifiedModels.end())
        {
            specifiedModels.push_back(this->modelFileName);
        }
    }

    void StaticObject::addPosition(const glm::vec3 &position)
    {
        positions.push_back(position);
    }

    const std::string &StaticObject::getModelFileName() const
    {
        return modelFileName;
    }

    const std::vector<std::string> &StaticObject::getSpecifiedModels()
    {
        return specifiedModels;
    }

    const std::vector<glm::vec3> &StaticObject::getPositions() const
    {
        return positions;
    }
}