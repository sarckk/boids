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

struct UpdateBoidPositionParams {
    int alignDist {};
    int alignMag {};
    int attractDist {};
    int attractMag {};
    int repelDist {};
    int repelMag {};
    int speed {};
};

class Boid {
private:
    Vector2d velocity;
    Vector2d position;
    int speed;
    int mass;
    int angle;

public:
    explicit Boid(Vector2d position);

    void updatePosition(const std::vector<Boid>& boids, UpdateBoidPositionParams params);
    void move(const Vector2d& acceleration, int speed);

    Vector2d getPosition() const;
    Vector2d getVelocity() const;
};


#endif //BOIDS_BOID_H
