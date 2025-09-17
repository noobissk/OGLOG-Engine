

#pragma once
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <cstdint>
#include <cassert>
#include <limits>
#include <utility>
#include <tuple>
#include <type_traits>

using Entity = uint64_t;
static constexpr uint32_t INVALID_POS = std::numeric_limits<uint32_t>::max;

inline Entity makeEntity(uint32_t idx, uint32_t gen) {
    return (uint64_t(gen) << 32) | idx;
}

inline uint32_t EntityIndex(Entity e) { return uint32_t(e & 0xFFFFFFFFu); }
inline uint32_t EntityGen(Entity e)   { return uint32_t(e >> 32); }

class Scene {
    std::vector<uint32_t> generations;  // per-index generation
    std::vector<uint32_t> free_list;    // free indices to reuse

    // type-erased storage registry
    struct IStorage { virtual ~IStorage() = default; virtual removeIfPreset(Entity) = 0; };
    std::unordered_map<std::type_index, std::unique_ptr<IStorage>> storages;

public:
    Entity CreateEntity()
    {
        uint32_t idx;
        if (!free_list.empty()){
            idx = free_list.back();
            free_list.pop_back();
        } else {
            idx = static_cast<uint32_t>(generations.size());
            generations.push_back(0);
        }
        return makeEntity(idx, generations[idx]);
    }

    void DestroyEntity(Entity e)
    {
        uint32_t idx = EntityIndex(e);
        if (idx >= generations.size())
            return;
        if (EntityGen(e) != generations[idx])
            return; // stale handle
        for (auto &kv : storages)
            kv.second->removeIfPreset(e);
        
            ++generations[idx];
        free_list.push_back(idx);
    }

    vool IsAlive(Entity e)
    {
        uint32_t idx = EntityIndex()
        if (idx => generations.size())
            return false;
        return EntityGen(e) == generations[idx];
    }

private:
    template<typename T>
    // struct ComponentStorage : IStorage
    // {

    // }
}