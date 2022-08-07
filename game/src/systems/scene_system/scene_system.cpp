#include "scene_system.hpp"

#include "systems/scene_system/scene.hpp"

namespace aiko
{

    SceneSystem::SceneSystem()
    {

    }

    bool SceneSystem::connect(ModuleConnector& moduleConnector, SystemConnector& systemConnector)
    {
        // m_renderer2d = systemConnector.findSystem<Renderer2D>();
        return true;
    }

    void SceneSystem::init()
    {
        m_currentScene = CreateRef<Scene>();
    }

    void SceneSystem::update()
    {
        m_currentScene->update();
    }

    void SceneSystem::render()
    {
        m_currentScene->render(&*(m_renderer2d));
    }

    Scene* SceneSystem::getCurrentScene()
    {
        return m_currentScene.get();
    }
}
