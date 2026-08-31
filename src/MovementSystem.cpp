#include "MovementSystem.hpp"
#include "World.hpp"
#include "Components.hpp"
#include "MathFunctions.hpp"

namespace systems {

    void updateMovement(World &world, float dt) {
        auto &positions = world.storage<Position>();
        auto &velocities = world.storage<Velocity>();

        using std::uint32_t;

        for (uint32_t i = 0; i < positions.getSize(); ++i) {
            Entity entity = positions.getEntityAt(i);
            if (!velocities.has(entity)) {
                continue;
            }

            Position &pos = positions.get(entity);
            Velocity &velocity = velocities.get(entity);
            pos.value += math::normalize(velocity.vector) * velocity.speed * dt;
        }
    }
}
