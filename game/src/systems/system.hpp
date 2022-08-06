#pragma once

#include "systems/system_connector.hpp"
#include "modules/module_connector.hpp"

namespace aiko
{

    class System
    {

    public:

        System() = default;
        virtual ~System() = default;
        virtual bool connect(ModuleConnector& moduleConnector, SystemConnector& systemConnector) { return true; };

        virtual void init() = 0;

        virtual void update() { };
        virtual void render() { };

    };

}
