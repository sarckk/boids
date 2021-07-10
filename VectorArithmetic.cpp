//
// Created by Yong Hoon Shin on 10/07/2021.
//

#include <cmath>
#include <SFML/System.hpp>

#include "VectorArithmetic.h"

float VectorArithmetic::getDistance(sf::Vector2f v1, sf::Vector2f v2) {
    float d_x = v1.x - v2.x;
    float d_y = v1.y - v2.y;
    return static_cast<float>(sqrt(d_x * d_x + d_y * d_y));
}

float VectorArithmetic::magnitude(sf::Vector2f& vec)  {
    return static_cast<float>(sqrt(vec.x * vec.x + vec.y * vec.y));
}

void VectorArithmetic::normalize(sf::Vector2f &vec) {
    float m = magnitude(vec);
    if(m > 0.) {
        vec /= m;
    }
}

void VectorArithmetic::limit(sf::Vector2f& vec, float limit)  {
    float m = magnitude(vec);
    if(m > 0 && m > limit) {
        normalize(vec);
        vec *= limit;
    }
}

void VectorArithmetic::setMag(sf::Vector2f &vec, float mag) {
    normalize(vec);
    vec *= mag;
}

