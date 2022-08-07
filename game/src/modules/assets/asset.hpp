#pragma once

#include <string>

#include "utils/uuid.h"

namespace aiko
{

    class Asset
    {
    public:
        Asset(const std::string path);
        virtual ~Asset();
        virtual bool load() = 0;
        const UUID& getUUID();
    protected:
        const std::string m_path;
        const UUID m_uuid;
    };

}