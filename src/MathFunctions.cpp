#include "MathFunctions.hpp"

namespace math {

    float computeVectorLength(sf::Vector2f vector) {
        return std::sqrt(vector.x * vector.x + vector.y * vector.y);
    }

    sf::Vector2f normalize(sf::Vector2f vector) {
        if (vector == sf::Vector2f{0.f, 0.f}) {
            return {0.f, 0.f};
        }
        float length = computeVectorLength(vector);
        vector.x /= length;
        vector.y /= length;
        return vector;
    }

}