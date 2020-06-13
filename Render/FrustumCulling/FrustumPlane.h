//
// Created by binybrion on 4/14/20.
//

#ifndef GLFW_FRUSTUMPLANE_H
#define GLFW_FRUSTUMPLANE_H

namespace Render
{
    namespace FrustumCulling
    {
        /**
         * Provides identifiers for the planes that make up a frustum.
         */

        enum FrustumPlane
        {
            Left = 0,
            Right = 1,
            Bottom = 2,
            Top = 3,
            Near = 4,
            Far = 5
        };
    }
}

#endif //GLFW_FRUSTUMPLANE_H
