#include "asset_module.hpp"

#include "modules/assets/assets.hpp"
#include "utils/raylib_include.hpp"

#include <algorithm>

namespace aiko
{

    AssetModule::~AssetModule()
    {

    }

    void AssetModule::init()
    {
        // TODO remove/move this
        // this->load<FontAsset>("resources/mecha.png");
    }

    void AssetModule::postInit()
    {
        
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
        assert(false);
        return nullptr;
    }

}