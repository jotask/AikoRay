#pragma once

#include <vector>

namespace aiko
{

    class Module;
    class System;

    class Aiko
    {
    public:
        static Aiko& getInstance() { static Aiko instance; return instance; }
    private:
        Aiko() = default;
        ~Aiko();
        Aiko(Aiko const&) = delete;
        void operator=(Aiko const&) = delete;
    public:
        void run();

    private:
        void init();

        std::vector<Module*> m_modules;
        std::vector<System*> m_systems;

    };

}