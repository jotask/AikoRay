#pragma once

#include "systems/system.hpp"
#include "systems/entity_component_system/entity_types.hpp"

#include <vector>
#include <map>

namespace aiko
{

    class Component;
    class Entity;
    class ImguiSystem;

    class EntityComponentSystem : public System
    {
    public:

        EntityComponentSystem();
        virtual ~EntityComponentSystem() override = default;

        virtual void init() override;
        virtual bool connect(ModuleConnector&, SystemConnector&) override;

        virtual void update() override;

        virtual void render() override;

        Entity& getEntityById(const entity::EntityId);
        Entity& getEntityByIdInTag(const entity::EntityId, const entity::EntityTag tag);

        entity::EntityCollection& getAllEntitiesByTag(const entity::EntityTag tag);

        void refresh();

        Entity& addEntity(const entity::EntityTag tag = entity::EntityTag::DEFAULT);

        void removeEntity(const entity::EntityId);

    private:

        std::map<entity::EntityTag, entity::EntityCollection> m_entities;

        ImguiSystem* m_imguiSystem;
        bool m_renderImgui;

    };

}
