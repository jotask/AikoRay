#include "ending_state.hpp"

#include "raylib.h"

#include "systems/states/options_state.hpp"

namespace aiko
{

    void EndingState::init()
    {
        framesCounter = 0;
        finishScreen = 0;
    }

    void EndingState::dispose()
    {

    }

    void EndingState::update()
    {
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
        {
            finishScreen = 1;
            // PlaySound(fxCoin);
        }
    }

    void EndingState::render()
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLUE);
        // DrawTextEx(font, "ENDING SCREEN", { 20, 10 }, font.baseSize * 3.0f, 4, DARKBLUE);
        DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);
    }

    State* EndingState::getNextState()
    {
        return nullptr;
    }

}