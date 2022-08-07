#include "asset.hpp"

namespace aiko
{

    Asset::Asset(const std::string path)
        : m_uuid()
        , m_path(path)

    {
    }

    Asset::~Asset()
    {

    }

    const UUID& Asset::getUUID()
    {
        return m_uuid;
    }

}