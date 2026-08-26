#pragma once
#include <vector>
#include <World.hpp>

namespace systems {

    std::vector<std::pair<Entity, Entity>> updateCollisions(World &world);

}