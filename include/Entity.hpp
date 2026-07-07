#pragma once
#include "SFML/Graphics.hpp"

class Entity : public sf::Drawable {

public:
    Entity(const sf::Texture &texture);
    ~Entity();

    struct PhysicsData {
        enum Shape {
            Circle,
            Rectangle
        };

        enum Type {
            Static,
            Dynamic
        };

        Shape shape;
        Type type;

        float radius;
        float width;
        float height;
        float speed;
    };

    PhysicsData physicsData_;

    void setScale(sf::Vector2f scale);
    void setOrigin(sf::Vector2f origin);
    void setPosition(sf::Vector2f position);
    void setSpeed(float speed);
    sf::Vector2f getPosition();

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void move(sf::Vector2f movement, sf::View &camera);

private:
    sf::Sprite sprite_;
};
