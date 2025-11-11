#pragma once
#inlcude <filesystem>


enum AssetType {
    System,
    Component,
    Shader,
    Texture,
    Model,
    Data,
    Other
}


class Asset {
public:
    AssetType type;
    std::path path;
}