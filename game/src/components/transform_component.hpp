#pragma once

#include "components/component.hpp"

namespace raylib
{
    #include <raylib.h>
}

namespace aiko
{

    class TransformComponent : public Component
    {
    public:
        raylib::Vector3 position = { 0.0f };
        raylib::Vector3 rotation = { 0.0f };
        raylib::Vector3 scale = { 1.0f };

        TransformComponent() : Component("TransformComponent") { };
        TransformComponent(const TransformComponent&) = default;

        // TODO
        /*
        glm::mat4 getTransformMatrix()
        {
            glm::mat4 rot = glm::toMat4(glm::quat(rotation));
            return glm::translate(glm::mat4(1.0f), position) * rot * glm::scale(glm::mat4(1.0f), scale);
        }
        */

    };

}
