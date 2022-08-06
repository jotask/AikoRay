#pragma once

#include "system.hpp"

namespace aiko
{

    class State;
    class GameStateSystem: public System
    {
    public:

        GameStateSystem() = default;
        virtual ~GameStateSystem();

        virtual void init() override;

        virtual void update() override;
        virtual void render() override;

    private:
        float transAlpha = 0.0f;
        bool onTransition = false;
        bool transFadeOut = false;

        State* m_transToScreen;
        State* m_currentState;

        void changeToScreen(State* nextScreen);     // Change to screen, no transition effect
        void transitionToScreen(State* nextScreen); // Request transition to next screen
        void updateTransition(void);         // Update transition effect
        void drawTransition(void);           // Draw transition effect (full-screen rectangle)


    };

}