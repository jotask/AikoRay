#include "input.hpp"

namespace aiko
{

    Input::Input()
    {

    }

    bool Input::IsKeyPressed()
    {
        return true; // IsKeyDown(key);
    }

    bool Input::IsMouseButtonPressed()
    {
        return true; // IsMouseButtonDown(button);
    }

    Vector2 Input::GetMousePosition()
    {
        return {}; // GetMousePosition();
    }

}
