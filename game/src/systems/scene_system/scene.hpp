#pragma once

#include <entt.hpp>

#include "systems/scene_system/entity.hpp"

#include <string>

namespace aiko
{

    class Renderer2D;

    class Scene
    {
    public:

        Scene() = default;

        virtual ~Scene() = default;

        Entity createEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void update();
        void render(Renderer2D* renderer);

        entt::registry& getEntityRegister() { return m_entityRegister; }

    private:

        friend class Entity;

        entt::registry m_entityRegister;

    };

}
