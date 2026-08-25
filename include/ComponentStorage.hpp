#pragma once
#include <vector>
#include <limits>
#include <cstdint>

using Entity = std::uint32_t;
constexpr Entity EMPTY = std::numeric_limits<std::uint32_t>::max();

template<typename T>
class ComponentStorage {

public:
    void remove(Entity id);
    void add(Entity id, T component);
    T& get(Entity id);
    Entity getEntityAt(std::uint32_t index);
    bool has(Entity id) const;
    std::uint32_t getSize() const;

private:
    std::vector<T> components_;
    std::vector<Entity> entities_;
    std::vector<std::uint32_t> sparse_;
};





