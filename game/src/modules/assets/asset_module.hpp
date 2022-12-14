#pragma once

#include "modules/module.hpp"

#include "modules/assets/asset.hpp"

#include <map>
#include <core/uuid.h>

namespace aiko
{

    class AssetModule : public Module
    {
    public:

        AssetModule() = default;
        virtual ~AssetModule();

        virtual void init() override;
        virtual void postInit();

        virtual void update() override;
        virtual void render() override;

        virtual void beginFrame() override;
        virtual void endFrame() override;

        template<class T, typename... Args>
        const UUID load(Args... args);

        template<class T>
        T* getAsset(const UUID& uuid);

    private:

        std::vector<Asset*> m_assets;

        Asset* getAssetInternal(const UUID& uuid);

    };

    template<class T, typename... Args>
    inline const UUID AssetModule::load( Args... args )
    {
        auto* ass = new T(args...);
        m_assets.emplace_back( ass );
        ass->load();
        return ass->getUUID();
    }

    template<class T>
    inline T* AssetModule::getAsset(const UUID& uuid)
    {
        static_assert(std::is_base_of<Asset, T>::value, "Derived not derived from Asset");
        auto* ass = getAssetInternal(uuid);
        if (ass == nullptr)
        {
            std::exception("Asset not found");
            return nullptr;
        }

        if (T* tmp = dynamic_cast<T*>(ass))
        {
            return tmp;
        }
        else
        {
            std::exception("Asset found is not of type requested");
            return nullptr;
        }

    }

}