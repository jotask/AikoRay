#pragma once

namespace aiko
{

    class State
    {

    public:

        State() = default;
        virtual ~State() = default;

        virtual void init() = 0;
        virtual void dispose() = 0;

        virtual void update() { };
        virtual void render() { };

        virtual bool isFinished() = 0;
        virtual State* getNextState() = 0;

    };

}
