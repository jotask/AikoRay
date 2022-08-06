#pragma once

#include "module.hpp"

namespace aiko
{

    class DisplayModule : public Module
    {
    public:

        DisplayModule() = default;
        virtual ~DisplayModule();

        static const int ScreenWidth = 800;
        static const int ScreenHeight = 450;

        virtual void init() override;

        virtual void update() override;
        virtual void render() override;

        virtual void beginFrame() override;
        virtual void endFrame() override;

    };

}