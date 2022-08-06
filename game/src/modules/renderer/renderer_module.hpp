#pragma once

#include "modules/module.hpp"

namespace aiko
{

    class RendererModule : public Module
    {
    public:

        RendererModule() = default;
        virtual ~RendererModule();

        virtual void init() override;

        virtual void update() override;
        virtual void render() override;

        virtual void beginFrame() override;
        virtual void endFrame() override;

    };

}