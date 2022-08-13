#include "game_state.hpp"

#include "utils/raylib_include.hpp"
#include "systems/game_state_system/states/options_state.hpp"

namespace aiko
{

    void GameState::init()
    {
        framesCounter = 0;
        finishScreen = 0;
    }

    void GameState::dispose()
    {

    }

    void GameState::update()
    {
        if (raylib::IsKeyPressed(raylib::KEY_ENTER) || raylib::IsGestureDetected(raylib::GESTURE_TAP))
        {
            finishScreen = true;
            // PlaySound(fxCoin);
        }
    }

    void GameState::render()
    {
        raylib::DrawRectangle(0, 0, raylib::GetScreenWidth(), raylib::GetScreenHeight(), raylib::PURPLE);
        // DrawTextEx(font, "GAMEPLAY SCREEN", { 20, 10 }, font.baseSize * 3.0f, 4, MAROON);
        raylib::DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, raylib::MAROON);
    }

    State* GameState::getNextState()
    {
        return new OptionsState();
    }

}