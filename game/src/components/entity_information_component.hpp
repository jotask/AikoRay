#pragma once

#include <string>

#include "components/component.hpp"

namespace aiko
{

    class EntityInformation : public Component
    {
    public:
        std::string name;

        EntityInformation(const EntityInformation&) = default;
        EntityInformation(const std::string& name) : Component("EntityInformation"), name(name) { }

    };

}
