#ifndef BOIDS_VECTORARITHMETIC_H
#define BOIDS_VECTORARITHMETIC_H


namespace VectorArithmetic {
    float getDistance(const sf::Vector2f& v1, const sf::Vector2f& v2);
    float getSquaredDistance(const sf::Vector2f& v1, const sf::Vector2f& v2);
    float magnitude(const sf::Vector2f& vec);
    void normalize_(sf::Vector2f& vec);
    void limit_(sf::Vector2f& vec, float limit);
    void setMag_(sf::Vector2f& vec, float mag);
    sf::Vector2f normalizeCopy(sf::Vector2f vec);
};


#endif //BOIDS_VECTORARITHMETIC_H
