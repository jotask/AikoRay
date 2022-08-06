#include "title_state.hpp"

#include <raylib.h>

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
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
        {
            finishScreen = true;
            // PlaySound(fxCoin);
        }
    }

    void TitleState::render()
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);
        // DrawTextEx(font, "TITLE SCREEN", { 20, 10 }, font.baseSize * 3.0f, 4, DARKGREEN);
        DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
    }

    State* TitleState::getNextState()
    {
        return new GameState();
    }

}