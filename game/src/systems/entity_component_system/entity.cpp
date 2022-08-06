#include "entity.hpp"

#include "systems/entity_component_system/components/component.hpp"

#include <algorithm>

namespace aiko
{

    Entity::Entity()
        : m_id(getEntityId())
    {

    }

    Transform & Entity::getTransform()
    {
        return m_transform;
    }

    void Entity::update()
    {
        std::for_each(m_components.begin(), m_components.end(), [](auto& component) { component->update(); });
    }

    void Entity::render()
    {
        std::for_each(m_components.begin(), m_components.end(), [](auto& component) { component->render(); });
    }

    bool Entity::isActive() const
    {
        return m_active;
    }

    void Entity::destroy()
    {
        m_active = false;
    }

    entity::EntityId Entity::getId() const
    {
        return m_id;
    }

}

