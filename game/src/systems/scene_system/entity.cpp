#include "entity.hpp"

namespace aiko
{
    Entity::Entity()
        : m_entityHandle(entt::null)
    {

    }

    Entity::Entity(EntityID id, Scene* scene)
        : m_entityHandle(id)
        , m_scene(scene)
    {

    }

}
