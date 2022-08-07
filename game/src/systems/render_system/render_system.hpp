#pragma once

#include <stack>

#include "systems/system.hpp"

namespace aiko
{

    class RenderSystem: public System
    {
    public:

        RenderSystem();
        virtual ~RenderSystem() = default;

        virtual bool connect(ModuleConnector& moduleConnector, SystemConnector& systemConnector) override;

        virtual void init() override;

        virtual void update() override;
        virtual void render() override;

    private:

    };

}
