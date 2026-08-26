#include "CollisionSystem.hpp"
#include "Components.hpp"

namespace systems {
    std::vector<std::pair<Entity, Entity>> updateCollisions(World &world) {
        auto &colliders = world.storage<Collider>();
        auto &positions = world.storage<Position>();
        std::vector<std::pair<Entity, Entity>> collisionPairs;

        using std::uint32_t;
        for (uint32_t i = 0; i < colliders.getSize(); ++i) {
            Entity firstEntity = colliders.getEntityAt(i);
            if (firstEntity == EMPTY) {
                continue;
            }
            assert(
                positions.has(firstEntity) &&
                "Entity with collider must have a position");
            for (uint32_t j = i + 1; j < colliders.getSize(); ++j) {
                Entity secondEntity = colliders.getEntityAt(j);
                if (secondEntity == EMPTY) {
                    continue;
                }
                assert(
                    positions.has(secondEntity) &&
                    "Entity with collider must have a position");
                sf::FloatRect first = colliders.get(firstEntity).rect;
                first.position += positions.get(firstEntity).value;
                sf::FloatRect second = colliders.get(secondEntity).rect;
                second.position += positions.get(secondEntity).value;
                if (first.findIntersection(second).has_value()) {
                    collisionPairs.emplace_back(firstEntity, secondEntity);
                }
            }
        }

        return collisionPairs;
    }
}
