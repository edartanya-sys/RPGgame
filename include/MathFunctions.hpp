#pragma once
#include <SFML/System/Vector2.hpp>

namespace math {

    float computeVectorLength(sf::Vector2f vector);
    sf::Vector2f normalize(sf::Vector2f vector);

}