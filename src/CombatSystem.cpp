#include "CombatSystem.hpp"
#include "Components.hpp"

namespace systems {
    void updateCombat(World &world, std::vector<std::pair<Entity, Entity>> &collisions) {
        auto &playerComp = world.storage<Player>();
        auto &enemies = world.storage<Enemy>();
        auto &healths = world.storage<HealthComponent>();
        auto &inv = world.storage<Invulnerability>();

        for (auto &[a, b] : collisions) {
            Entity player = playerComp.has(a) ? a : playerComp.has(b) ? b : EMPTY;
            if (player == EMPTY) {
                continue;
            }
            Entity other = (player == a) ? b : a;
            if (!enemies.has(other)) {
                continue;
            }

            if (inv.has(player)) {
                continue;
            }

            healths.get(player).value -= 10;
            inv.add(player, Invulnerability{1.f});
        }
    }

    void updateInvulnerability(World& world, float dt) {
        auto& inv = world.storage<Invulnerability>();
        for (std::int32_t i = inv.getSize() - 1; i >= 0; --i) {
            Entity e = inv.getEntityAt(i);
            inv.get(e).timeLeft -= dt;
            if (inv.get(e).timeLeft <= 0.f) {
                inv.remove(e);
            }
        }
    }

}
