#pragma once

#include <string>

#include "core/uuid.h"

namespace aiko
{

    class Asset
    {
    public:
        Asset(const std::string path);
        virtual ~Asset();
        virtual bool load() = 0;
        const UUID& getUUID();
        const std::string& getPath();
    protected:
        const std::string m_path;
        const UUID m_uuid;
    };

}