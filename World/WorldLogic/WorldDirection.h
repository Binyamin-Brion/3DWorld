//
// Created by binybrion on 4/16/20.
//

#ifndef GLFW_WORLDDIRECTION_H
#define GLFW_WORLDDIRECTION_H

namespace World
{
    namespace WorldLogic
    {
        /**
         * Represents directions in the game world that are used for loading new parts of the world.
         */

        enum class WorldDirection
        {
            North,
            East,
            South,
            West,
        };
    }
}

#endif //GLFW_WORLDDIRECTION_H
