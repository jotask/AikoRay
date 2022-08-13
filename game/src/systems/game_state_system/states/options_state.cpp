#include "options_state.hpp"

#include "utils/raylib_include.hpp"
#include "systems/game_state_system/states/ending_state.hpp"

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
        if (raylib::IsKeyPressed(raylib::KEY_ENTER) || raylib::IsGestureDetected(raylib::GESTURE_TAP))
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