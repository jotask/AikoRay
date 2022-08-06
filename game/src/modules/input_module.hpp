#pragma once

#include "module.hpp"

#include "raylib.h"

namespace aiko
{

    class InputModule : public Module
    {
    public:

        InputModule() = default;
        virtual ~InputModule();

        virtual void init() override;

        virtual void update() override;
        virtual void render() override;

        virtual void beginFrame() override;
        virtual void endFrame() override;

    private:

    };

}