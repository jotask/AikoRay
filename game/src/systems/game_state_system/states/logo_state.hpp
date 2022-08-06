#pragma once

#include "state.hpp"

namespace aiko
{

    class LogoState : public State
    {

    public:

        LogoState() = default;
        virtual ~LogoState() = default;

        virtual void init() override;
        virtual void dispose() override;

        virtual void update() override;
        virtual void render() override;

        virtual bool isFinished() override { return finishScreen; };
        virtual State* getNextState() override;

    private:

        int framesCounter = 0;
        bool finishScreen = false;

        int logoPositionX = 0;
        int logoPositionY = 0;

        int lettersCount = 0;

        int topSideRecWidth = 0;
        int leftSideRecHeight = 0;

        int bottomSideRecWidth = 0;
        int rightSideRecHeight = 0;

        int state = 0;              // Logo animation states
        float alpha = 1.0f;         // Useful for fading

    };

}
