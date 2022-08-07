#pragma once

#include <string>

namespace aiko
{

    class Component
    {
    protected:
        Component() = delete;
        Component(std::string name) : m_componentName(name) { }
        Component(const Component&) = default;

        virtual ~Component() = default;

    public:
        std::string getComponentName() const { return m_componentName; };

    private:
        std::string m_componentName;
    };

}
