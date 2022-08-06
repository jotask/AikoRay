#include "game_state_system.hpp"

#include "states/state.hpp"

#include "states/logo_state.hpp"

#include <raylib.h>

namespace aiko
{

    GameStateSystem::~GameStateSystem()
    {

    }

    void GameStateSystem::init()
    {
        SetTargetFPS(60);
        m_currentState = new LogoState();
    }

    void GameStateSystem::update()
    {

        if ( onTransition == false )
        {
            m_currentState->update();

            if (m_currentState->isFinished() == true)
            {
                transitionToScreen(m_currentState->getNextState());
            }
        }
        else
        {
            updateTransition();    // Update transition (fade-in, fade-out)
        }
        
    }

    void GameStateSystem::render()
    {
        m_currentState->render();

        if (onTransition == true)
        {
            drawTransition();
        }

    }

    void GameStateSystem::changeToScreen(State* nextScreen)
    {
        // Unload current screen
        m_currentState->dispose();

        // Init next screen
        nextScreen->init();

        m_currentState = nextScreen;
    }

    void GameStateSystem::transitionToScreen(State* nextScreen)
    {
        onTransition = true;
        transFadeOut = false;
        m_transToScreen = nextScreen;
        transAlpha = 0.0f;
    }

    void GameStateSystem::updateTransition(void)
    {
        if (!transFadeOut)
        {
            transAlpha += 0.05f;

            // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
            // For that reason we compare against 1.01f, to avoid last frame loading stop
            if (transAlpha > 1.01f)
            {
                transAlpha = 1.0f;

                // Unload current screen
                m_currentState->dispose();

                // Load next screen
                m_transToScreen->init();

                m_currentState = m_transToScreen;

                // Activate fade out effect to next loaded screen
                transFadeOut = true;
            }
        }
        else  // Transition fade out logic
        {
            transAlpha -= 0.02f;

            if (transAlpha < -0.01f)
            {
                transAlpha = 0.0f;
                transFadeOut = false;
                onTransition = false;
                m_transToScreen = m_currentState;
            }
        }
    }

    void GameStateSystem::drawTransition(void)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
    }

}