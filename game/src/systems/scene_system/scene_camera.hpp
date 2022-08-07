#pragma once

#include "modules/renderer/camera.hpp"

namespace aiko
{

    class SceneCamera : public Camera
    {
    public:
        SceneCamera() = default;
        virtual ~SceneCamera() = default;
    private:
    };

}