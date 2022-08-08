#include "Aiko.hpp"

#include "core/math.hpp"

#include "modules/display/display_module.hpp"
#include "modules/audio/audio_module.hpp"
#include "modules/renderer/renderer_module.hpp"
#include "modules/assets/asset_module.hpp"
#include "modules/input/input_module.hpp"

#include "systems/game_state_system/game_state_system.hpp"
#include "systems/scene_system/scene_system.hpp"
#include "systems/render_system/render_system.hpp"

#include "events/engine_events.hpp"
#include "events/event_system.hpp"

#include <algorithm>

namespace aiko
{

    Aiko::Aiko()
        : m_isRunning(true)
    {

    }

    void Aiko::init()
    {

        // Modules

        m_modules.emplace_back(CreateScope<AssetModule>());
        m_modules.emplace_back(CreateScope<DisplayModule>());
        m_modules.emplace_back(CreateScope<RendererModule>());
        m_modules.emplace_back(CreateScope<InputModule>());
        m_modules.emplace_back(CreateScope<AudioModule>());

        // Connect all system before we initialize them
        auto moduleConnector = ModuleConnector(m_modules);
        std::for_each(m_modules.begin(), m_modules.end(), [&moduleConnector](auto& m) { if (m->connect(moduleConnector) == false) { /* spdlog::error("Error Connecting system!");*/ std::terminate(); }; });

        std::for_each(m_modules.begin(), m_modules.end(), [](auto& m) { m->init(); });

        // TODO : Fix this this is used now to force load all assets on start. Ideally the scene should be the responsible to load everything needed?
        std::for_each(m_modules.begin(), m_modules.end(), [](auto& m) { m->postInit(); });

        // Systems

        m_systems.emplace_back(CreateScope<RenderSystem>());
        m_systems.emplace_back(CreateScope<SceneSystem>());
        m_systems.emplace_back(CreateScope<GameStateSystem>());

        auto systemConnector = SystemConnector(m_systems);
        std::for_each(m_systems.begin(), m_systems.end(), [&moduleConnector, &systemConnector](auto& s) { if (s->connect(moduleConnector, systemConnector) == false) { /* spdlog::error("Error Connecting system!");*/ std::terminate(); }; });

        std::for_each(m_systems.begin(), m_systems.end(), [](auto& s) { s->init(); });

        EventSystem::it().bind<WindowCloseEvent>(this, &Aiko::onWindowClosedEventHandler);

    }

    void Aiko::onWindowClosedEventHandler(Event& evnt)
    {
        const auto& windowClose = static_cast <WindowCloseEvent&> (evnt);
        m_isRunning = false;
    }

    Aiko::~Aiko()
    {

    }

    void Aiko::run()
    {
        init();
        while (m_isRunning == true )
        {

            std::for_each(m_modules.begin(), m_modules.end(), [](auto& m) { m->update(); });
            std::for_each(m_systems.begin(), m_systems.end(), [](auto& s) { s->update(); });

            std::for_each(m_modules.begin(), m_modules.end(), [](auto& m) { m->beginFrame(); });
            std::for_each(m_modules.begin(), m_modules.end(), [](auto& m) { m->render(); });
            std::for_each(m_systems.begin(), m_systems.end(), [](auto& s) { s->render(); });
            std::for_each(m_modules.begin(), m_modules.end(), [](auto& m) { m->endFrame(); });

        }
    }

}

int main(void)
{
    auto& aiko = aiko::Aiko::getInstance();
    aiko.run();
}