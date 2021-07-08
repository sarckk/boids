//
// Created by Yong Hoon Shin on 06/07/2021.
//

#ifndef BOIDS_BOID_H
#define BOIDS_BOID_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "Vector2d.h"

class Boid {
private:
    Vector2d velocity;
    Vector2d position;
    int alignDist;
    int attractDist;
    int repelDist;
    int speed;
    int mass;
    int angle;
    int radius;

public:
    explicit Boid(Vector2d position);

    void updatePosition(const std::vector<Boid>& boids);
    void move(const Vector2d& acceleration);

    Vector2d getPosition() const;
    Vector2d getVelocity() const;
};


#endif //BOIDS_BOID_H
