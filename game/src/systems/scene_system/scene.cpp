#include "scene.hpp"

#include "systems/scene_system/all_components_include.hpp"

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

    void Scene::render(Renderer2D* renderer)
    {

        // Render 2D
        /*
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto view = m_entityRegister.view<TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.isPrimary == true)
                {
                    mainCamera = &camera.camera;
                    cameraTransform = transform.getTransformMatrix();
                    break;
                }
            }
        }

        if (mainCamera != nullptr)
        {
            renderer->beginScene(*mainCamera, cameraTransform);
        }

        auto group = m_entityRegister.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            renderer->drawSprite(transform.getTransformMatrix(), sprite, (int)entity);
        }

        if (mainCamera != nullptr)
        {
            renderer->endScene();
        }

        */

    }

}
