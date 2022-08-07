#include "scene_system.hpp"

#include "systems/scene_system/scene.hpp"
#include "systems/render_system/render_system.hpp"

#include "components/camera_component.hpp"
#include "components/transform_component.hpp"

namespace aiko
{

    SceneSystem::SceneSystem()
    {

    }

    bool SceneSystem::connect(ModuleConnector& moduleConnector, SystemConnector& systemConnector)
    {
        m_renderer = systemConnector.findSystem<RenderSystem>();
        return true;
    }

    void SceneSystem::init()
    {
        m_currentScene = CreateRef<Scene>();

        // Create default camera for scene
        {
            auto camera = m_currentScene->createEntity("Default Camera");
            camera.addComponent<CameraComponent>();
            camera.getComponent<TransformComponent>().position = { 10.0f, 10.0f, 10.0f };
        }

    }

    void SceneSystem::update()
    {
        m_currentScene->update();
    }

    void SceneSystem::render()
    {
        m_currentScene->render(&*(m_renderer));
    }

    Scene* SceneSystem::getCurrentScene()
    {
        return m_currentScene.get();
    }
}
