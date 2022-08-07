#pragma once

#include <cstdint>

#include <entt.hpp>

#include "core/assert.hpp"

namespace aiko
{

    class Scene;

    class Entity
    {
    public:

        using EntityID = entt::entity;

        Entity();
        Entity(EntityID  id, Scene* scene);
        Entity(const Entity& other) = default;

        template<class T, typename... Args>
        T& addComponent(Args&&... args)
        {
            AIKO_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
            T& component = m_scene->m_entityRegister.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
            return component;
        }

        template<class T>
        T& getComponent()
        {
            AIKO_CORE_ASSERT(!HasComponent<T>(), "Entity does not have component!");
            return m_scene->m_entityRegister.get<T>(m_entityHandle);
        }

        template<class T>
        bool hasComponent()
        {
            return m_scene->m_entityRegister.has<T>(m_entityHandle);
        }

        template<class T>
        T& removeComponent()
        {
            AIKO_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            return m_scene->m_entityRegister.remove<T>(m_entityHandle);
        }

        operator bool() const { return m_entityHandle != entt::null; }
        operator entt::entity() const { return m_entityHandle; }
        operator uint32_t() const { return (uint32_t)m_entityHandle; }
        bool operator==(const Entity& other) const { return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene; }
        bool operator!=(const Entity& other) const { return !(*this == other); }

    private:

        EntityID m_entityHandle;
        Scene* m_scene;

    };

}
