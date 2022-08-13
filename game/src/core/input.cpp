#include "input.hpp"

namespace aiko
{

    Input::Input()
    {

    }

    bool Input::isKeyPressed(const raylib::KeyboardKey key)
    {
        return IsKeyDown(key);
    }

    bool Input::isMouseButtonPressed(const raylib::MouseButton button)
    {
        return raylib::IsMouseButtonDown(button);
    }

    Vector2 Input::getMousePosition()
    {
        return raylib::GetMousePosition();
    }

}
