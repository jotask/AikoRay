#pragma once

#include "state.hpp"

namespace aiko
{

    class OptionsState : public State
    {

    public:

        OptionsState() = default;
        virtual ~OptionsState() = default;

        virtual void init() override;
        virtual void dispose() override;

        virtual void update() override;
        virtual void render() override;

        virtual bool isFinished() override { return finishScreen; };
        virtual State* getNextState() override;

    private:

        int framesCounter = 0;
        bool finishScreen = false;

    };

}
