#include "EnemyAISystem.hpp"

#include "World.hpp"
#include "Components.hpp"
#include "MathFunctions.hpp"
#include <cassert>
#include <iostream>

namespace systems {
    void updateEnemyAI(World &world) {
        auto &enemies = world.storage<Enemy>();
        auto &chaseComponents = world.storage<ChaseComponent>();
        auto &velocities = world.storage<Velocity>();
        auto &positions = world.storage<Position>();

        using std::uint32_t;

        // TODO сделать отдельную функцию для получения игрока
        Entity player = world.storage<Player>().getEntityAt(0);
        sf::Vector2f playerPos = positions.get(player).value;
        for (uint32_t i = 0; i < enemies.getSize(); ++i) {
            Entity enemy = enemies.getEntityAt(i);
            if (!chaseComponents.has(enemy)) {
                continue;
            }
            assert(
                velocities.has(enemy) &&
                "Entity with ChaseComponent must have Velocity");

            sf::Vector2f direction = math::normalize(
                playerPos - positions.get(enemy).value);

            velocities.get(enemy).vector = direction;
            std::cout << direction.x << ' ' << direction.y << '\n';
        }
    }
}
