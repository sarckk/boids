#include <cmath>
#include <SFML/System.hpp>

#include "VectorArithmetic.h"

float VectorArithmetic::getDistance(const sf::Vector2f& v1, const sf::Vector2f& v2) {
    return static_cast<float>(sqrt(getSquaredDistance(v1,v2)));
}

float VectorArithmetic::getSquaredDistance(const sf::Vector2f& v1, const sf::Vector2f& v2) {
    float d_x = v1.x - v2.x;
    float d_y = v1.y - v2.y;
    return d_x * d_x + d_y * d_y ;
}

float VectorArithmetic::magnitude(const sf::Vector2f& vec) {
    return static_cast<float>(sqrt(vec.x * vec.x + vec.y * vec.y));
}

void VectorArithmetic::normalize_(sf::Vector2f &vec) {
    float m = magnitude(vec);
    if(m > 0.) {
        vec /= m;
    }
}

sf::Vector2f VectorArithmetic::normalizeCopy(sf::Vector2f vec) {
    float m = magnitude(vec);
    if(m > 0.) {
        vec /= m;
    }
    return vec;
}

void VectorArithmetic::limit_(sf::Vector2f& vec, float limit)  {
    float m = magnitude(vec);
    if(m > 0 && m > limit) {
        normalize_(vec);
        vec *= limit;
    }
}

void VectorArithmetic::setMag_(sf::Vector2f &vec, float mag) {
    normalize_(vec);
    vec *= mag;
}

