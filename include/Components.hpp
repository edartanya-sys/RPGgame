#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

struct Position {
    sf::Vector2f value;
};

struct Velocity {
    sf::Vector2f vector;
    float speed;
};

struct Collider {
    sf::FloatRect rect;
};

struct SpriteComponent {
    sf::Sprite sprite;
};

struct Player {};

struct Enemy {};

struct ChaseComponent {};

struct PatrolComponent {
    sf::Vector2f point1;
    sf::Vector2f point2;
};

struct HealthComponent {
    int value;
};