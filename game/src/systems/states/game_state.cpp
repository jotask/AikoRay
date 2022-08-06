#include "game_state.hpp"

#include "raylib.h"

#include "systems/states/options_state.hpp"

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
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
        {
            finishScreen = true;
            // PlaySound(fxCoin);
        }
    }

    void GameState::render()
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
        // DrawTextEx(font, "GAMEPLAY SCREEN", { 20, 10 }, font.baseSize * 3.0f, 4, MAROON);
        DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
    }

    State* GameState::getNextState()
    {
        return new OptionsState();
    }

}