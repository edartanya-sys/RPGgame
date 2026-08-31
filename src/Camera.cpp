#include "Camera.hpp"

void Camera::setSize(sf::Vector2f size) {
    view_.setSize(size);
}

void Camera::setCenter(sf::Vector2f center) {
    view_.setCenter(center);
}

void Camera::move(sf::Vector2f offset) {
    view_.move(offset);
}

void Camera::update(sf::Vector2f windowBounds, sf::Vector2f windowHalfSize, sf::Vector2f cameraPos) {
    if (cameraPos.x <= windowHalfSize.x) {
        cameraPos.x = windowHalfSize.x;
    }
    else if (cameraPos.x >= windowBounds.x) {
        cameraPos.x = windowBounds.x;
    }
    if (cameraPos.y <= windowHalfSize.y) {
        cameraPos.y = windowHalfSize.y;
    }
    else if (cameraPos.y >= windowBounds.y) {
        cameraPos.y = windowBounds.y;
    }
    setCenter(cameraPos);
}

const sf::View &Camera::getView() {
    return view_;
}

