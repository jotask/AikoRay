#pragma once

#include <vector>

namespace aiko
{

    class System;

    class SystemConnector
    {
    public:

        using Systems = std::vector<System*>;

        class SystemConnector (Systems& systems)
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

        Systems& m_systems;

    };

}
