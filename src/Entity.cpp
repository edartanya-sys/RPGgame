#include "Entity.hpp"
#include "SFML/Graphics.hpp"

Entity::Entity(const sf::Texture &texture)
    : sprite_{texture} {

}

Entity::~Entity() {}


void Entity::setScale(sf::Vector2f scale) {
    sprite_.setScale(scale);
}

void Entity::setOrigin(sf::Vector2f origin) {
    sprite_.setOrigin(origin);
}

void Entity::setPosition(sf::Vector2f position) {
    sprite_.setPosition(position);
}

void Entity::setSpeed(float speed) {
    physicsData_.speed = speed;
}

sf::Vector2f Entity::getPosition() {
    return sprite_.getPosition();
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(sprite_);
}

void Entity::move(sf::Vector2f movement, sf::View &camera) {
    camera.move(movement);
    sprite_.move(movement);
}
