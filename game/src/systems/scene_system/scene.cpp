#include "scene.hpp"

#include "systems/scene_system/all_components_include.hpp"
#include "systems/render_system/render_system.hpp"

namespace aiko
{

    Entity Scene::createEntity(const std::string& name)
    {
        Entity entity = { m_entityRegister.create(), this };
        auto& uuid = entity.addComponent<IDComponent>();
        auto& entityInformation = entity.addComponent<EntityInformation>(name.empty() ? "Entity" : name);
        auto& transform = entity.addComponent<TransformComponent>();
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_entityRegister.destroy(entity);
    }

    void Scene::update()
    {

    }

    void Scene::render(RenderSystem* renderer)
    {

        // Render 2D
        Camera* mainCamera = nullptr;
        TransformComponent* transformComponent;
        auto view = m_entityRegister.view<TransformComponent, CameraComponent>();
        for (auto entity : view)
        {
            auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

            if (camera.isPrimary == true)
            {
                mainCamera = &camera.camera;
                transformComponent = &transform;
                break;
            }
        }

        if (mainCamera != nullptr)
        {
            renderer->beginScene(mainCamera, transformComponent);
        }

        /*
        auto group = m_entityRegister.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            renderer->drawSprite(transform.getTransformMatrix(), sprite, (int)entity);
        }
        */

        if (mainCamera != nullptr)
        {
            renderer->endScene();
        }

    }

}
