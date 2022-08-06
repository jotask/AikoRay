#include "Aiko.hpp"

#include "modules/display_module.hpp"
#include "modules/audio_module.hpp"
#include "modules/renderer_module.hpp"
#include "modules/font_module.hpp"
#include "modules/input_module.hpp"

#include "systems/game_state_system.hpp"

#include <algorithm>

namespace aiko
{

    void Aiko::init()
    {

        // Modules

        m_modules.emplace_back(new DisplayModule());
        m_modules.emplace_back(new InputModule());
        m_modules.emplace_back(new RendererModule());
        m_modules.emplace_back(new AudioModule());
        m_modules.emplace_back(new FontModule());

        // Connect all system before we initialize them
        auto moduleConnector = ModuleConnector(m_modules);
        std::for_each(m_modules.begin(), m_modules.end(), [&moduleConnector](auto& m) { if (m->connect(moduleConnector) == false) { /* spdlog::error("Error Connecting system!");*/ std::terminate(); }; });

        std::for_each(m_modules.begin(), m_modules.end(), [](Module* m) { m->init(); });

        // Systems

        m_systems.emplace_back(new GameStateSystem());

        auto systemConnector = SystemConnector(m_systems);
        std::for_each(m_systems.begin(), m_systems.end(), [&moduleConnector, &systemConnector](auto& s) { if (s->connect(moduleConnector, systemConnector) == false) { /* spdlog::error("Error Connecting system!");*/ std::terminate(); }; });

        std::for_each(m_systems.begin(), m_systems.end(), [](System* s) { s->init(); });

    }

    Aiko::~Aiko()
    {
        std::for_each(m_modules.begin(), m_modules.end(), [](Module* m) { delete m; });
        std::for_each(m_systems.begin(), m_systems.end(), [](System* s) { delete s; });
    }

    void Aiko::run()
    {
        init();
        while ( WindowShouldClose() == false )    // Detect window close button or ESC key
        {

            std::for_each(m_modules.begin(), m_modules.end(), [](Module* m) { m->update(); });
            std::for_each(m_systems.begin(), m_systems.end(), [](System* s) { s->update(); });

            std::for_each(m_modules.begin(), m_modules.end(), [](Module* m) { m->beginFrame(); });
            std::for_each(m_modules.begin(), m_modules.end(), [](Module* m) { m->render(); });
            std::for_each(m_systems.begin(), m_systems.end(), [](System* s) { s->render(); });
            std::for_each(m_modules.begin(), m_modules.end(), [](Module* m) { m->endFrame(); });

        }
    }

}

int main(void)
{
    auto& aiko = aiko::Aiko::getInstance();
    aiko.run();
}