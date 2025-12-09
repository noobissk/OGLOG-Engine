#pragma once
#include <unordered_map>
#include <string>
#include <filesystem>
#include <iostream>

using Asset = uint16_t;
namespace fs = std::filesystem;


class AssetManager {
public:
    static void start(bool build_assets = false);
    static std::vector<std::string> readFileFromId(Asset id);
    static fs::path projectDirectory();
    static Asset pathToAsset(const fs::path& path);
    static fs::path assetToPath(Asset id);
    static std::unordered_map<uint16_t, fs::path> asset_map;
    static const fs::path assets_dir_rel_path;
    static const fs::path asset_db_rel_path;
};