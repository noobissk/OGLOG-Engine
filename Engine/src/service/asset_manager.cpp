#include "asset_manager.h"
#include <vector>
#include <map>
#include <unordered_set>

const fs::path AssetManager::assets_rel_path = "../assets";;


void AssetManager::start()
{
    std::vector<std::string> all_assets;
    
    for (const auto& entry : fs::recursive_directory_iterator(assets_rel_path)) {
        if (fs::is_regular_file(entry.path())) {
            // get the relative path from base_dir
            fs::path relative = fs::relative(entry.path(), assets_rel_path);

            std::cout << "[LOG] Registering Asset -> " << relative.string() << '\n';
            all_assets.push_back(relative.string());
        }
    }


    std::unordered_set<std::string> desiredSet(all_assets.begin(), all_assets.end());

    
}