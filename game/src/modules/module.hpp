#pragma once

#include "module_connector.hpp"

namespace aiko
{

    class Module
    {

    public:

        Module() = default;
        virtual ~Module() = default;

        virtual void init() = 0;
        virtual void postInit() { };

        virtual bool connect(ModuleConnector& moduleConnector) { return true; };

        virtual void update() { };
        virtual void render() { };

        virtual void beginFrame() { };
        virtual void endFrame() { };

    };

}
