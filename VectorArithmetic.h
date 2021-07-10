//
// Created by Yong Hoon Shin on 10/07/2021.
//

#ifndef BOIDS_VECTORARITHMETIC_H
#define BOIDS_VECTORARITHMETIC_H


namespace VectorArithmetic {
    float getDistance(sf::Vector2f v1, sf::Vector2f v2);
    float magnitude(sf::Vector2f& vec);
    void normalize(sf::Vector2f& vec);
    void limit(sf::Vector2f& vec, float limit);
    void setMag(sf::Vector2f& vec, float mag);
};


#endif //BOIDS_VECTORARITHMETIC_H
