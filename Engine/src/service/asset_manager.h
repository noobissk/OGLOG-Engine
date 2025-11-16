#pragma once
#include "service.h"
#include <unordered_map>
#include <string>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;


class AssetManager {
public:
    static void start();
    static std::unordered_map<uint16_t, std::string> asset_map;
    static const fs::path assets_dir_rel_path;
    static const fs::path asset_db_rel_path;
};