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
    sf::Vector2f size;
};

struct SpriteComponent {
    sf::Sprite sprite;
};

struct Player {};