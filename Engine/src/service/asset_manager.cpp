#include "asset_manager.h"
#include <vector>
#include <map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <util/string_operations.h>

const fs::path AssetManager::assets_dir_rel_path = "../../assets";
const fs::path AssetManager::asset_db_rel_path = "../../assets.sdb";
std::unordered_map<uint16_t, fs::path> AssetManager::asset_map;


std::vector<std::string> read_all_file_lines(fs::path path_to_file) {
    std::vector<std::string> all_lines;

    if (!fs::exists(path_to_file)) {
        std::cout << "[Error] Invalid filepath -> " << path_to_file << std::endl;
        return all_lines; // empty vector
    }

    std::ifstream MyReadFile(path_to_file);
    std::string myText;
    while (getline(MyReadFile, myText)) {
        all_lines.push_back(myText);
    }
    return all_lines;
}

Asset AssetManager::pathToAsset(const fs::path& p)
{
    for (const auto& [id, mapped_path] : asset_map) {
        if (mapped_path == p)
            return id;
    }
    throw std::out_of_range("Asset path not found in asset_map");
}
fs::path AssetManager::assetToPath(Asset asset_id) {
    if (asset_id == 0)
        return fs::path();
    return projectDirectory() / ".." / "assets"/ asset_map.at(asset_id);
}

fs::path AssetManager::projectDirectory() {
    return fs::current_path().parent_path();
}

void AssetManager::start()
{
    std::cout << "[LOG] AssetManager::start() - Current working directory: " << fs::current_path() << std::endl;
    std::cout << "[LOG] Looking for assets at: " << fs::absolute(assets_dir_rel_path) << std::endl;
    std::cout << "[LOG] Database file path: " << fs::absolute(asset_db_rel_path) << std::endl;
    
    std::vector<std::string> all_assets;
    
    // Check if assets directory exists
    if (!fs::exists(assets_dir_rel_path)) {
        std::cout << "[ERROR] Assets directory does not exist: " << fs::absolute(assets_dir_rel_path) << std::endl;
        return;
    }
    
    for (const auto& entry : fs::recursive_directory_iterator(assets_dir_rel_path)) {
        if (fs::is_regular_file(entry.path())) {
            // get the relative path from base_dir
            fs::path relative = fs::relative(entry.path(), assets_dir_rel_path);

            std::cout << "[LOG] Registering Asset -> " << relative.string() << '\n';
            all_assets.push_back(relative.string());
        }
    }

    std::cout << "[LOG] Found " << all_assets.size() << " assets" << std::endl;

    std::vector<std::string> file_lines = read_all_file_lines(asset_db_rel_path);
    std::unordered_map<std::string, int> db_lines;

    for (std::string& line : file_lines) { // read existing database
        std::pair<std::string, std::string> pair = string_split(line, " -> ");
        db_lines.emplace(pair.first, std::stoi(pair.second));
    }

    std::vector<uint16_t> existing_values; // read paths in assets
    std::unordered_map<std::string, int> merged_map;
    for (const std::string path : all_assets) {
        auto it = db_lines.find(path);
        int value = -1;
        if (it != db_lines.end()) {
            value = it->second;
            existing_values.push_back(value);
        }
        merged_map.emplace(path, value);
    }


    
    for (auto& [key, value] : merged_map) {
        uint16_t nextUnique = 1;
        if (value == -1) {
            for (size_t i = 0; i < existing_values.size(); i++) {
                ++nextUnique;
            }
            value = nextUnique;
            existing_values.push_back(nextUnique);
        }
    }

    std::ofstream simple_database(asset_db_rel_path);
    
    if (!simple_database.is_open()) {
        std::cout << "[ERROR] Failed to open database file for writing: " << fs::absolute(asset_db_rel_path) << std::endl;
        return;
    }

    for (const auto& pair : merged_map)
    {
        simple_database << pair.first << " -> " << pair.second << std::endl;
        asset_map[pair.second] = pair.first;
    }
    simple_database.close();
    std::cout << "[LOG] Asset database written successfully to: " << fs::absolute(asset_db_rel_path) << std::endl;
}
