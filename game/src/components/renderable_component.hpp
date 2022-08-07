#pragma once

#include "components/component.hpp"

namespace aiko
{

    class RenderableComponent : public Component
    {
    protected:
        RenderableComponent() = delete;
        RenderableComponent(std::string name) : Component(name) { }
        RenderableComponent(const RenderableComponent&) = default;
    public:
        virtual void render() = 0;
    };

}
