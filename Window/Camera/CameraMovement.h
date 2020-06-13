//
// Created by BinyBrion on 8/11/2019.
//

#ifndef GAMEEDITOR_CAMERAMOVEMENT_H
#define GAMEEDITOR_CAMERAMOVEMENT_H

namespace Window::Camera
{
    /**
     *  Represents the possible movements that a camera can make.
     *  Note that these can be combined, e.g Left-Forward is the result
     *  of two calls to moving the camera- Left and Forwards
     */

    enum class CameraMovement
    {
        Backwards,
        Forwards,
        Left,
        Right
    };
}

#endif //GAMEEDITOR_CAMERAMOVEMENT_H
