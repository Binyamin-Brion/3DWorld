//
// Created by binybrion on 6/28/20.
//

#ifndef GAMEPROTOTYPE_STATICOBJECT_H
#define GAMEPROTOTYPE_STATICOBJECT_H

#include <string>
#include <vec3.hpp>
#include <vector>

namespace Objects
{
    /**
     * Stores the translations associated with a static model.
     */

    class StaticObject
    {
        public:

            /**
             * Initializes this object with the model it represents.
             *
             * @param modelFileName location on the file system of the model this object represents
             */
            explicit StaticObject(std::string modelFileName);

            /**
             * Get the positions associated with the static model.
             *
             * @param position vector of positions
             */
            void addPosition(const glm::vec3& position);

            /**
             * Get the name of the model file that this object is representing
             *
             * @return name of the model file that this object is representing
             */
            [[nodiscard]] const std::string& getModelFileName() const;

            /**
             * Get the name of all of the models that are being represented by instances of this class
             *
             * @return vector holding the names of all static models being represented
             */
            [[nodiscard]] static const std::vector<std::string>& getSpecifiedModels();

            /**
             * Get the positions associated with this static model.
             *
             * @return vector of positions
             */
            [[nodiscard]] const std::vector<glm::vec3>& getPositions() const;

        private:

            static std::vector<std::string> specifiedModels;

            std::string modelFileName;
            std::vector<glm::vec3> positions;
    };
}

#endif //GAMEPROTOTYPE_STATICOBJECT_H
