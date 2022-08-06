#pragma once

#include "utils/shared.hpp"
#include "utils/transform.hpp"
#include "systems/entity_component_system/entity_types.hpp"

#include <vector>
#include <array>
#include <bitset>

namespace aiko
{

    class Component;

    class Entity
    {
    private:

        static entity::EntityId getEntityId()
        {
            static auto lastId = entity::EntityId(0u);
            return lastId++;
        }

    public:

        static entity::ComponentId getComponentId()
        {
            static auto lastId = entity::ComponentId(0u);
            return lastId++;
        }

        template<class T>
        entity::ComponentId getComponentTypeId() const noexcept
        {
            static auto typeId = getComponentId();
            return typeId;
        }

        constexpr static std::size_t maxComponents = 32;
        using CompomentBitSet = std::bitset<maxComponents>;
        using ComponentArray = std::array<Component*, maxComponents>;

        Entity();
        virtual ~Entity() = default;

        bool m_active;
        std::vector<AikoUPtr<Component>> m_components;

        ComponentArray m_componentArray;
        CompomentBitSet m_componentBitSet;

        Transform m_transform;

        Transform& getTransform();

        void update();
        void render();

        bool isActive() const;

        void destroy();

        entity::EntityId getId() const;

        const entity::EntityId m_id;

        template<class T>
        bool hasComponent() const
        {
            return m_componentBitSet[getComponentTypeId<T>()];
        }

        template<class T, class... TArgs>
        T& addComponent(TArgs&&... args)
        {
            T* c(new T(this, std::forward<TArgs>(args)...));
            AikoUPtr<Component> ptr{ c };
            m_components.emplace_back(std::move(ptr));

            m_componentArray[getComponentTypeId<T>()] = c;
            m_componentBitSet[getComponentTypeId<T>()] = true;

            c->init();

            return *c;

        }

        template<class T>
        T& getComponent() const
        {
            auto ptr(m_componentArray[getComponentTypeId<T>()]);
            return *static_cast<T*>(ptr);
        }

    };

}
