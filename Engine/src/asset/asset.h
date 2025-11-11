#pragma once
#include <string>

enum class AssetType {
    Texture,
    Audio,
    Model,
    Custom
};

struct Asset {
    int id;
    std::string path;
    AssetType type;
};
