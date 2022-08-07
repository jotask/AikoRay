#pragma once

#include <raylib.h>

#include "core/singleton.hpp"

namespace aiko
{

    class Input : public Singleton<Input>
    {
        friend class InputModule;
    private:
        Input();
        ~Input() = default;
    public:
        bool isKeyPressed(const KeyboardKey key);
        bool isMouseButtonPressed(const MouseButton button);
        Vector2 getMousePosition();

    };
}
