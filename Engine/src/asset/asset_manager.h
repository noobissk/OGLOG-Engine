#include <unordered_map>

class AssetManager {
public:
    AssetID RegisterAsset(const std::string& path, AssetType type) {
        AssetID id = nextID++;
        assets[id] = { id, path, type };
        pathToID[path] = id;
        return id;
    }

    const std::string& GetPath(AssetID id) const {
        return assets.at(id).path;
    }

    AssetID GetID(const std::string& path) const {
        return pathToID.at(path);
    }

private:
    using AssetID = int;
    int nextID = 1;
    std::unordered_map<AssetID, Asset> assets;
    std::unordered_map<std::string, AssetID> pathToID;
};
