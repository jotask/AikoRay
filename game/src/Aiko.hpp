#pragma once

#include "utils/raylib_include.hpp"

#include "core/application_types.h"
#include "events/event.hpp"

namespace aiko
{

    class Module;
    class System;

    class Aiko
    {
    public:
        static Aiko& getInstance() { static Aiko instance; return instance; }
    private:
        Aiko();
        ~Aiko();
        Aiko(Aiko const&) = delete;
        void operator=(Aiko const&) = delete;
    public:
        void run();

    private:
        void init();

        ModulesCollection m_modules;
        SystemCollection m_systems;

        bool m_isRunning;

        void onWindowClosedEventHandler(Event&);

    };

}