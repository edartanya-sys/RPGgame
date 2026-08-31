#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/View.hpp"
#include "World.hpp"


class Camera {
public:

    void setSize(sf::Vector2f size);
    void setCenter(sf::Vector2f center);
    void move(sf::Vector2f offset);
    void update(sf::Vector2f windowBounds, sf::Vector2f windowHalfSize, sf::Vector2f cameraPos);
    const sf::View &getView();

private:
    sf::View view_{};

};

