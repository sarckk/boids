//
// Created by Yong Hoon Shin on 06/07/2021.
//

#ifndef BOIDS_BOID_H
#define BOIDS_BOID_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "Arrow.h"

struct UpdateBoidPositionParams {
    int alignDist {};
    float alignWeight {};
    int attractDist {};
    float attractWeight {};
    int repelDist {};
    float repelWeight {};
    int maxSpeed {};
};

class Boid : public Arrow {
private:
    sf::Vector2f velocity;
    int speed;
    float mass;
    int angle;

public:
    explicit Boid(sf::Vector2f position, sf::Vector2f velocity);

    void updateVelocity(const std::vector<Boid>& boids, UpdateBoidPositionParams params);

    void moveBounded(unsigned int windowWidth, unsigned int windowHeight, unsigned int margin);
    sf::Vector2f getVelocity() const;
};


#endif //BOIDS_BOID_H
