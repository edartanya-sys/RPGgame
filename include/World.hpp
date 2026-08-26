#pragma once
#include <cstdint>
#include "ComponentStorage.hpp"
#include "Components.hpp"

using Entity = std::uint32_t;

class World {

public:
    Entity create();
    void destroy(Entity);
    bool isAlive(Entity) const;

    template<typename T>
    void addComponent(Entity, T);

    template<typename T>
    ComponentStorage<T>& storage();

private:

    Entity nextEntity_ = 0;
    std::vector<Entity> freeEntityList_{};
    std::vector<std::uint8_t> aliveEntities_;

    ComponentStorage<Position> positions_{};
    ComponentStorage<Velocity> velocities_{};
    ComponentStorage<SpriteComponent> sprites_{};
    ComponentStorage<Collider> colliders_{};
    ComponentStorage<Player> controllables_{};
    ComponentStorage<Enemy> enemies_{};
    ComponentStorage<ChaseComponent> chaseComponents_{};
    ComponentStorage<PatrolComponent> patrolComponents_{};
    ComponentStorage<HealthComponent> healths_{};
};

extern template void World::addComponent<Position>(Entity, Position);
extern template void World::addComponent<Velocity>(Entity, Velocity);
extern template void World::addComponent<SpriteComponent>(Entity, SpriteComponent);
extern template void World::addComponent<Collider>(Entity, Collider);
extern template void World::addComponent<Player>(Entity, Player);
extern template void World::addComponent<Enemy>(Entity, Enemy);
extern template void World::addComponent<ChaseComponent>(Entity, ChaseComponent);
extern template void World::addComponent<PatrolComponent>(Entity, PatrolComponent);
extern template void World::addComponent<HealthComponent>(Entity, HealthComponent);