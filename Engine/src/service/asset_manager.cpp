#include "asset_manager.h"
#include <vector>
#include <map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <util/string_operations.h>

const fs::path AssetManager::assets_dir_rel_path = "../assets";
const fs::path AssetManager::asset_db_rel_path = "../assets.sdb";


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

void AssetManager::start()
{
    std::vector<std::string> all_assets;
    
    for (const auto& entry : fs::recursive_directory_iterator(assets_dir_rel_path)) {
        if (fs::is_regular_file(entry.path())) {
            // get the relative path from base_dir
            fs::path relative = fs::relative(entry.path(), assets_dir_rel_path);

            std::cout << "[LOG] Registering Asset -> " << relative.string() << '\n';
            all_assets.push_back(relative.string());
        }
    }


    std::vector<std::string> file_lines = read_all_file_lines(asset_db_rel_path);
    std::unordered_map<std::string, int> db_lines;

    for (std::string& line : file_lines) {
        std::pair<std::string, std::string> pair = string_split(line, " -> ");
        db_lines.emplace(pair.first, std::stoi(pair.second));
    }

    std::vector<uint16_t> existing_values;
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


    uint16_t nextUnique = 0;

    for (auto& [key, value] : merged_map) {
        if (value == -1) {
            while (std::find(existing_values.begin(), existing_values.end(), nextUnique) != existing_values.end()) {
                ++nextUnique;
            }
            value = nextUnique;
            existing_values.push_back(nextUnique);
        }
    }

    std::ofstream simple_database(asset_db_rel_path);

    for (const auto& pair : merged_map)
    {
        simple_database << pair.first << " -> " << pair.second << std::endl;
    }
    simple_database.close();
}
