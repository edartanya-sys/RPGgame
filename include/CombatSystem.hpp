#pragma once
#include <vector>
#include <World.hpp>

namespace systems {
    void updateCombat(World &world,
                      std::vector<std::pair<Entity, Entity>> &collisions);

    void updateInvulnerability(World &world, float dt);
}