#pragma once

#include <string>
#include <xhash>

namespace aiko
{

    class UUID
    {
    public:
        UUID();
        UUID(uint64_t uuid);
        UUID(const UUID&) = default;
        bool operator == (const UUID& other) { return m_UUID == other.m_UUID; }
        operator uint64_t() const { return m_UUID; }
        operator std::string() const { return std::to_string(m_UUID); }
    private:
        uint64_t m_UUID;
    };
}


namespace std {

    template<>
    struct hash<aiko::UUID>
    {
        std::size_t operator()(const aiko::UUID& uuid) const
        {
            return hash<uint64_t>()((uint64_t)uuid);
        }
    };

}

