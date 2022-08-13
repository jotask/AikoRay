#include "ending_state.hpp"

#include "utils/raylib_include.hpp"
#include "systems/game_state_system/states/options_state.hpp"

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
        if (raylib::IsKeyPressed(raylib::KEY_ENTER) || raylib::IsGestureDetected(raylib::GESTURE_TAP))
        {
            finishScreen = 1;
            // PlaySound(fxCoin);
        }
    }

    void EndingState::render()
    {
        raylib::DrawRectangle(0, 0, raylib::GetScreenWidth(), raylib::GetScreenHeight(), raylib::BLUE);
        // DrawTextEx(font, "ENDING SCREEN", { 20, 10 }, font.baseSize * 3.0f, 4, DARKBLUE);
        raylib::DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, raylib::DARKBLUE);
    }

    State* EndingState::getNextState()
    {
        return nullptr;
    }

}