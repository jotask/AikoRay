#pragma once

#include "utils/raylib_include.hpp"
#include "core/math.hpp"
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
        bool isKeyPressed(const raylib::KeyboardKey key);
        bool isMouseButtonPressed(const raylib::MouseButton button);
        Vector2 getMousePosition();

    };
}
