#include "World.hpp"
#include <type_traits>

Entity World::create() {
    Entity id;
    if (!freeEntityList_.empty()) {
        id = freeEntityList_.back();
        freeEntityList_.pop_back();
    }
    else {
        id = nextEntity_;
        ++nextEntity_;
        aliveEntities_.push_back(false);
    }

    aliveEntities_[id] = true;
    return id;
}

void World::destroy(Entity id) {
    if (!isAlive(id)) {
        return;
    }
    positions_.remove(id);
    velocities_.remove(id);
    sprites_.remove(id);
    colliders_.remove(id);
    controllables_.remove(id);

    aliveEntities_[id] = false;
    freeEntityList_.push_back(id);

}

bool World::isAlive(Entity id) const {
    return id < aliveEntities_.size() && aliveEntities_[id];
}

template<typename T>
void World::addComponent(Entity id, T component) {
    auto &compStorage = storage<T>();
    compStorage.add(id, component);
}

namespace {
    template<typename>
    inline constexpr bool alwaysFalse = false;
}

template<typename T>
ComponentStorage<T>& World::storage() {
    if constexpr (std::is_same_v<T, Position>) {
        return positions_;
    }
    else if constexpr (std::is_same_v<T, Velocity>) {
        return velocities_;
    }
    else if constexpr (std::is_same_v<T, SpriteComponent>) {
        return sprites_;
    }
    else if constexpr (std::is_same_v<T, Collider>) {
        return colliders_;
    }
    else if constexpr (std::is_same_v<T, Player>) {
        return controllables_;
    }
    else {
        static_assert(alwaysFalse<T>, "This component type is not registered in World");
    }
}

template void World::addComponent<Position>(Entity, Position);
template void World::addComponent<Velocity>(Entity, Velocity);
template void World::addComponent<SpriteComponent>(Entity, SpriteComponent);
template void World::addComponent<Collider>(Entity, Collider);
template void World::addComponent<Player>(Entity, Player);