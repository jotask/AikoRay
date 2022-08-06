#pragma once

#include "utils/shared.hpp"

#include <cstddef>
#include <vector>

namespace aiko
{

    class Entity;

    namespace entity
    {
        using EntityId = std::size_t;
        using ComponentId = std::size_t;
        using EntityCollection = std::vector<AikoUPtr<Entity>>;

        enum class EntityTag
        {
            DEFAULT,
            LIGHT,
            CHUNK
        };

    }
}
