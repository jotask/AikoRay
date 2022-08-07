#pragma once

#include "components/component.hpp"
#include "core/uuid.h"

namespace aiko
{

    class IDComponent : public Component
    {
    public:
        UUID id;
        IDComponent() : Component("IDComponent") {};
        IDComponent(const IDComponent&) = default;
    };

}
