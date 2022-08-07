#include "scene_system.hpp"

#include "systems/scene_system/scene.hpp"
#include "systems/render_system/render_system.hpp"

namespace aiko
{

    SceneSystem::SceneSystem()
    {

    }

    bool SceneSystem::connect(ModuleConnector& moduleConnector, SystemConnector& systemConnector)
    {
        // FIXME
        m_renderer = systemConnector.findSystem<RenderSystem>();
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
        m_currentScene->render(&*(m_renderer));
    }

    Scene* SceneSystem::getCurrentScene()
    {
        return m_currentScene.get();
    }
}
