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
    return assets_dir_rel_path / asset_map.at(asset_id);
}

fs::path AssetManager::projectDirectory() {
    return fs::current_path().parent_path();
}

void AssetManager::start(bool build_assets)
{
    std::cout << "[LOG] AssetManager::start()" << std::endl;
    
    // Always read the existing database
    std::vector<std::string> file_lines = read_all_file_lines(asset_db_rel_path);
    std::unordered_map<std::string, int> db_lines;

    for (std::string& line : file_lines) { // read existing database
        std::pair<std::string, std::string> pair = string_split(line, " -> ");
        db_lines.emplace(pair.first, std::stoi(pair.second));
    }
    
    // Populate asset_map from database
    for (const auto& [path, id] : db_lines) {
        asset_map[id] = path;
    }
    
    // Only scan and update if build_assets is true
    if (build_assets) {
        std::cout << "[LOG] AssetManager::start() - Building assets" << std::endl;
        std::cout << "[LOG] Looking for assets at: " << fs::absolute(assets_dir_rel_path) << std::endl;
        
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
                all_assets.push_back(relative.string());
            }
        }

        std::cout << "[LOG] Found " << all_assets.size() << " assets" << std::endl;

        std::unordered_set<uint16_t> used_ids;
        for (auto& [path, id] : db_lines) {
            used_ids.insert(id);
        }

        // Merge DB with file system
        std::unordered_map<std::string, uint16_t> merged;

        // Assign IDs
        for (const std::string& path : all_assets) {

            auto it = db_lines.find(path);

            if (it != db_lines.end()) {
                // existing file → keep same ID
                merged[path] = it->second;
                used_ids.insert(it->second);
            }
            else {
                // New asset → assign first free ID
                uint16_t new_id = 1;
                while (used_ids.count(new_id))
                    ++new_id;

                merged[path] = new_id;
                used_ids.insert(new_id);
            }
        }

        // mapt -> vector for sorting
        std::ofstream out(asset_db_rel_path);
        std::vector<std::pair<std::string, uint16_t>> sorted_entries(
            merged.begin(), merged.end()
        );

        // Sort
        std::sort(sorted_entries.begin(), sorted_entries.end(), [](auto& a, auto& b) {
            return a.first < b.first;
        });

        for (auto& [path, id] : sorted_entries) {
            out << path << " -> " << id << "\n";
            asset_map[id] = path;
        }

    }
}
