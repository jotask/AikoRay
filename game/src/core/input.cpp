#include "input.hpp"

namespace aiko
{

    Input::Input()
    {

    }

    bool Input::isKeyPressed(const KeyboardKey key)
    {
        return IsKeyDown(key);
    }

    bool Input::isMouseButtonPressed(const MouseButton button)
    {
        return IsMouseButtonDown(button);
    }

    Vector2 Input::getMousePosition()
    {
        return GetMousePosition();
    }

}
