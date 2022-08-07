#pragma once

#include "core/application_types.h"

namespace aiko
{

    class Module;

    class ModuleConnector
    {
    public:

        ModuleConnector(ModulesCollection& modules)
            : m_modules(modules)
        {

        }

        template <class T>
        T* findModule()
        {
            for (auto& m : m_modules)
            {
                if (T* tmp = dynamic_cast<T*>(m.get())) {
                    return tmp;
                }
            }
            return nullptr;
        }

    private:

        ModulesCollection& m_modules;

    };

}
