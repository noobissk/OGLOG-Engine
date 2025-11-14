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
    static std::unordered_map<std::string, uint16_t> asset_link;
    static const fs::path assets_rel_path;
};