#pragma once

#include "asset.hpp"

#include <raylib.h>

namespace aiko
{

    class FontAsset : public Asset
    {
    public:
        FontAsset(const std::string path);
        virtual ~FontAsset();
        virtual bool load() override;
    private:
        Font m_font = { 0 };
    };

}