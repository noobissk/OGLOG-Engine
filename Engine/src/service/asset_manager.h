#pragma once
#include <unordered_map>
#include <string>
#include <filesystem>
#include <iostream>

using Asset = uint16_t;
namespace fs = std::filesystem;


class AssetManager {
public:
    static void start();
    static std::vector<std::string> readFileFromId(Asset id);
    static fs::path projectDirectory();
    static fs::path pathToAsset(Asset id);
    static std::unordered_map<uint16_t, fs::path> asset_map;
    static const fs::path assets_dir_rel_path;
    static const fs::path asset_db_rel_path;
};