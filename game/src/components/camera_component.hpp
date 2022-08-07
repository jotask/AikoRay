#pragma once

#include "components/component.hpp"
#include "systems/scene_system/scene_camera.hpp"

namespace aiko
{

    class CameraComponent : public Component
    {
    public:

        SceneCamera camera;
        bool isPrimary = true; // TODO: think about moving to Scene
        bool fixedAspectRatio = false;

        CameraComponent() : Component("CameraComponent") { };
        CameraComponent(const CameraComponent&) = default;


    };

}
