#include "asset_module.hpp"

#include "modules/assets/assets.hpp"

#include <raylib.h>

#include <algorithm>

namespace aiko
{

    AssetModule::~AssetModule()
    {

    }

    void AssetModule::init()
    {
        this->load<FontAsset>("resources/mecha.png");

        std::for_each(m_assets.begin(), m_assets.end(), [](Asset* ass) { ass->load(); });

    }

    void AssetModule::update()
    {

    }

    void AssetModule::render()
    {

    }

    void AssetModule::beginFrame()
    {

    }

    void AssetModule::endFrame()
    {

    }

    Asset* AssetModule::getAssetInternal(const UUID& uuid)
    {
        for (auto* ass : m_assets )
        {
            if (ass->getUUID() == uuid)
            {
                return ass;
            }
        }
        std::exception("Asset not found");
        return nullptr;
    }

}