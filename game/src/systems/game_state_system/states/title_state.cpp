#include "title_state.hpp"

#include "utils/raylib_include.hpp"
#include "systems/game_state_system/states/game_state.hpp"

namespace aiko
{

    void TitleState::init()
    {
        framesCounter = 0;
        finishScreen = 0;
    }

    void TitleState::dispose()
    {

    }

    void TitleState::update()
    {
        if (raylib::IsKeyPressed(raylib::KEY_ENTER) || raylib::IsGestureDetected(raylib::GESTURE_TAP))
        {
            finishScreen = true;
            // PlaySound(fxCoin);
        }
    }

    void TitleState::render()
    {
        raylib::DrawRectangle(0, 0, raylib::GetScreenWidth(), raylib::GetScreenHeight(), raylib::GREEN);
        // DrawTextEx(font, "TITLE SCREEN", { 20, 10 }, font.baseSize * 3.0f, 4, DARKGREEN);
        raylib::DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, raylib::DARKGREEN);
    }

    State* TitleState::getNextState()
    {
        return new GameState();
    }

}