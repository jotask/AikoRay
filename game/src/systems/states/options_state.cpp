#include "options_state.hpp"

#include "raylib.h"

#include "systems/states/ending_state.hpp"

namespace aiko
{

    void OptionsState::init()
    {
        framesCounter = 0;
        finishScreen = 0;
    }

    void OptionsState::dispose()
    {

    }

    void OptionsState::update()
    {
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
        {
            finishScreen = true;
            // PlaySound(fxCoin);
        }
    }

    void OptionsState::render()
    {
       
    }

    State* OptionsState::getNextState()
    {
        return new EndingState();
    }

}