#include "assets.hpp"

namespace aiko
{
    FontAsset::FontAsset(const std::string path)
        : Asset(path)
    {

    }

    FontAsset::~FontAsset()
    {
        UnloadFont(m_font);
    }

    bool FontAsset::load()
    {
        m_font = LoadFont(m_path.c_str());
        // TODO
        return true;
    }

}