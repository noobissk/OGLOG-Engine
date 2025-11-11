#pragma once
#include <vector>
#include <string>
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
static constexpr uint32_t INVALID_POS = std::numeric_limits<uint32_t>::max();


inline Entity makeEntity(uint32_t idx, uint32_t gen) {
    return (uint64_t(gen) << 32) | idx;
}

inline uint32_t entityIndex(Entity e) { return uint32_t(e & 0xFFFFFFFFu); }
inline uint32_t entityGen(Entity e)   { return uint32_t(e >> 32); }

class Scene {
    std::vector<uint32_t> generations;  // per-index generation
    std::vector<uint32_t> free_list;    // free indices to reuse

    // type-erased storage registry
    struct IStorage { virtual ~IStorage() = default; virtual void removeIfPresent(Entity) = 0; };
    std::unordered_map<std::type_index, std::unique_ptr<IStorage>> storages;

    // uint16_t id;

public:
    std::string name;

    Scene(std::string name) : name(name) { }

    bool operator==(const Scene& other) {
        return name == other.name;
    }
    bool operator==(const std::string other) {
        return name == other;
    }

    Entity createEntity()
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
        uint32_t idx = entityIndex(e);
        if (idx >= generations.size())
            return;
        if (entityGen(e) != generations[idx])
            return; // stale handle
        for (auto &kv : storages)
            kv.second->removeIfPresent(e);
        
        ++generations[idx];
        free_list.push_back(idx);
    }

    bool isAlive(Entity e) const {
        uint32_t idx = entityIndex(e);
        if (idx >= generations.size()) return false;
        return entityGen(e) == generations[idx];
    }

private:
    template<typename T>
    struct ComponentStorage : IStorage
    {
        std::vector<T> components; // Component data
        std::vector<Entity> entities; // dense Entity handles (same order as components)
        std::vector<uint32_t> sparse; // maps entity index -> dense index (or INVALID_POS) | works as dictionary

        void ensureSparseSize(uint32_t index) {
            if (index >= sparse.size())
                sparse.resize(index + 1, INVALID_POS);
        }

        template<typename... Args>
        T& add(Entity e, Args&&... args) {
            uint32_t idx = entityIndex(e);
            ensureSparseSize(idx);
            uint32_t pos = sparse[idx];
            if (pos != INVALID_POS && entities[pos] == e) {
                // already present — overwrite
                components[pos] = T(std::forward<Args>(args)...);
                return components[pos];
            }
            pos = static_cast<uint32_t>(components.size());
            components.emplace_back(std::forward<Args>(args)...);
            entities.push_back(e);
            sparse[idx] = pos;
            return components[pos];
        }

        bool has(Entity e) const {
            uint32_t idx = entityIndex(e);
            if (idx >= sparse.size()) return false;
            uint32_t pos = sparse[idx];
            if (pos == INVALID_POS) return false;
            return entities[pos] == e;
        }

        T& get(Entity e) {
            assert(has(e) && "component not present");
            return components[sparse[entityIndex(e)]];
        }

        T* tryGet(Entity e) {
            if (!has(e)) return nullptr;
            return &get(e);
        }

        void remove(Entity e)
        {
            uint32_t idx = entityIndex(e);
            if (idx >= sparse.size())
                return;
            uint32_t pos = sparse[idx];
            if (pos == INVALID_POS)
                return;
            uint32_t last = static_cast<uint32_t>(components.size() - 1);
            if (pos != last) {
                components[pos] = std::move(components[last]);
                entities[pos]   = entities[last];

                sparse[entityIndex(entities[pos])] = pos;
            }

            components.pop_back();
            entities.pop_back();
            sparse[idx] = INVALID_POS;
            
        }

        void removeIfPresent(Entity e) override {
                remove(e);
            }

        const std::vector<Entity>& denseEntities() const {
            return entities;
        }

        size_t size() const { return components.size(); }

        
    };

    template<typename T>
    ComponentStorage<T>& getOrCreateStorage() {
        std::type_index key(typeid(T));
        auto it = storages.find(key);
        if (it == storages.end()) {
            auto ptr = std::make_unique<ComponentStorage<T>>();
            it = storages.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(key),
                std::forward_as_tuple(std::move(ptr))
            ).first;
        }
        return *static_cast<ComponentStorage<T>*>(it->second.get());
    }


public:
    template<typename T, typename... Args>
    T& addComponent(Entity e, Args&&... args) {
        assert(isAlive(e) && "addComponent on dead entity");
        return getOrCreateStorage<T>().add(e, std::forward<Args>(args)...);
    }

    template<typename T>
    bool hasComponent(Entity e) {
        return getOrCreateStorage<T>().has(e);
    }

    template<typename T>
    T& getComponent(Entity e) {
        return getOrCreateStorage<T>().get(e);
    }

    template<typename T>
    T* tryGetComponent(Entity e) {
        return getOrCreateStorage<T>().tryGet(e);
    }

    template<typename T>
    void removeComponent(Entity e) {
        getOrCreateStorage<T>().remove(e);
    }

    // --- Simple forEach (system) ---
    // Calls func(Entity, T&... ) for every entity that has all Components.
    template<typename... Components, typename Func>
    void forEach(Func&& func) {
        static_assert(sizeof...(Components) >= 1, "forEach needs >= 1 component type");
        using First = std::tuple_element_t<0, std::tuple<Components...>>;
        auto &base = getOrCreateStorage<First>();
        for (Entity e : base.denseEntities()) {
            // check all other components exist:
            if ((getOrCreateStorage<Components>().has(e) && ...)) {
                func(e, getOrCreateStorage<Components>().get(e)...);
            }
        }
    }
};