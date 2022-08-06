#pragma once

#include <raylib.h>

#include "utils/singleton.hpp"

namespace aiko
{

    class Input : public Singleton<Input>
    {
        friend class InputModule;
    private:
        Input();
        ~Input() = default;
    public:
        bool IsKeyPressed();
        bool IsMouseButtonPressed();
        Vector2 GetMousePosition();

    };
}
