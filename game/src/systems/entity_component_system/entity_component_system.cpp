#include "entity_component_system.hpp"

#include "systems/entity_component_system/entity.hpp"
#include "systems/entity_component_system/components/component.hpp"
#include "utils/shared.hpp"

#include <algorithm>

namespace aiko
{
    EntityComponentSystem::EntityComponentSystem()
        : m_renderImgui(false)
    {

    }

    bool EntityComponentSystem::connect(ModuleConnector& moduleConnector, SystemConnector& systemConnector)
    {
        return true;
    }

    void EntityComponentSystem::init()
    {

    }

    void EntityComponentSystem::update()
    {
        std::for_each(m_entities.begin(), m_entities.end(), [](auto& pair) {
            auto& collection = pair.second;
            std::for_each(collection.begin(), collection.end(), [](auto& entity) { entity->update(); });
        });
    }

    void EntityComponentSystem::render()
    {

        std::for_each(m_entities.begin(), m_entities.end(), [](auto& pair) {
            auto& collection = pair.second;
            std::for_each(collection.begin(), collection.end(), [](auto& entity) { entity->render(); });
        });

    }

    Entity& EntityComponentSystem::getEntityById(const entity::EntityId id)
    {
        // std::for_each(m_entities.begin(), m_entities.end(), [id](const std::pair<entity::EntityTag, entity::EntityCollection>& pair) {
        //     auto& collection = pair.second;
        //     auto& found = std::find_if(collection.begin(), collection.end(), [id](const std::unique_ptr<Entity>& e) { return id == e->getId(); });
        //     if (found != collection.end())
        //     {
        //         auto tmp = *found;
        //         return *(*found).get();
        //     }
        // });

        // TODO fix this using stl
        for (auto& entities : m_entities)
        {
            for (auto& e : entities.second)
            {
                if (e->getId() == id)
                {
                    return *e.get();
                }
            }
        }

        std::exception("Not entity found with id");
    }

    Entity & EntityComponentSystem::getEntityByIdInTag(const entity::EntityId id, const entity::EntityTag tag)
    {
        auto& collection = getAllEntitiesByTag(tag);
        auto found = std::find_if(collection.begin(), collection.end(), [id](const auto& e) { return id == e->getId(); });
        if (found != collection.end())
        {
            return **found;
        }
        std::exception("Not entity found with id in tag");
    }

    entity::EntityCollection& EntityComponentSystem::getAllEntitiesByTag(const entity::EntityTag tag)
    {
        if (std::any_of(m_entities.begin(), m_entities.end(), [tag](const auto& collection) { return collection.first == tag; }) == false)
        {
            std::exception("Not entity collection found for tag");
        }
        return m_entities.at(tag);
    }

    void EntityComponentSystem::refresh()
    {
        std::for_each(m_entities.begin(), m_entities.end(), [](auto& pair) {
            auto& collection = pair.second;
            collection.erase(std::remove_if(collection.begin(), collection.end(), [](const auto& entity) { return entity->isActive() == false; }), collection.end());
        });
    }

    Entity& EntityComponentSystem::addEntity(const entity::EntityTag tag)
    {
        if (std::any_of(m_entities.begin(), m_entities.end(), [tag](const auto& collection) { return collection.first == tag; }) == false)
        {
            m_entities.insert({tag, entity::EntityCollection()});
        }
        Entity* e = new Entity();
        AikoUPtr<Entity> ptr{ e };
        m_entities.at(tag).emplace_back(std::move(ptr));
        return *e;
    }

    void EntityComponentSystem::removeEntity(const entity::EntityId id)
    {
        for (auto& allTags : m_entities)
        {
            auto& collection = allTags.second;
            allTags.second.erase(std::remove_if(collection.begin(), collection.end(), [id](const AikoUPtr<Entity>& e) { return e->getId() == id; }), collection.end());
        }
    }

}
