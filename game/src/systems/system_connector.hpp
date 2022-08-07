#pragma once

#include "core/application_types.h"

namespace aiko
{

    class System;

    class SystemConnector
    {
    public:

        class SystemConnector (SystemCollection& systems)
            : m_systems(systems)
        {

        }

        template <class T>
        T* findSystem()
        {
            for (auto& m : m_systems)
            {
                if (T* tmp = dynamic_cast<T*>(m.get())) {
                    return tmp;
                }
            }
            return nullptr;
        }

    private:

        SystemCollection& m_systems;

    };

}
