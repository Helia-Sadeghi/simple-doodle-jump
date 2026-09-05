/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <SFML/Graphics/Font.hpp>

template <typename T>
class ResourceManager {
private:
    std::map<std::string, T> resources;

public:
    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    T& load(const std::string& id, const std::string& filename) {
        auto it = resources.find(id);
        if (it != resources.end()) {
            return it->second;
        }

        T resource;
        bool loaded;
        if constexpr (std::is_same_v<T, sf::Font>) {
            loaded = resource.openFromFile(filename);
        } else {
            loaded = resource.loadFromFile(filename);
        }

        if (!loaded) {
            throw std::runtime_error("ResourceManager: failed to load '" + filename + "'");
        }

        auto result = resources.emplace(id, std::move(resource));
        return result.first->second;
    }

    T& get(const std::string& id) {
        auto it = resources.find(id);
        if (it == resources.end()) {
            throw std::runtime_error("ResourceManager: resource '" + id + "' was not loaded");
        }
        return it->second;
    }

    bool contains(const std::string& id) const {
        return resources.find(id) != resources.end();
    }
};

#endif
