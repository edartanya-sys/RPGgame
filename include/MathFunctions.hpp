#pragma once
#include <SFML/System/Vector2.hpp>

namespace math {

    float computeVectorLength(sf::Vector2f vector);
    sf::Vector2f normalize(sf::Vector2f vector);

    template<typename T> requires std::is_enum_v<T>
    constexpr size_t toIndex(T id) {
        return static_cast<size_t>(id);
    }

}