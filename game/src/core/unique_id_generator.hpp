#pragma once

#include <cstdint>

namespace aiko
{

    class IdGenerator
    {
    private:
        IdGenerator()
            : m_id(0)
        {

        }

        ~IdGenerator() = default;

        uint32_t m_id;

    public:

        static IdGenerator& get()
        {
            static IdGenerator gen;
            return gen;
        }

        static uint32_t next()
        {
            return get().m_id++;
        }

    };

}
