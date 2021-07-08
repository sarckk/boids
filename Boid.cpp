//
// Created by Yong Hoon Shin on 06/07/2021.
//

#include <cmath>
#include <assert.h>
#include <iostream>
#include "Boid.h"
#include "Vector2d.h"

Vector2d Boid::getPosition() const {
    return position;
}

Vector2d Boid::getVelocity() const {
    return velocity;
};

float getBoidDistance(const Boid& a, const Boid& b) {
    float d_x = a.getPosition().x - b.getPosition().x;
    float d_y = a.getPosition().y - b.getPosition().y;
    return sqrt(d_x * d_x + d_y * d_y);
}

void Boid::updatePosition(const std::vector<Boid>& boids) {
    Vector2d steer {};
    Vector2d align, attract, repel;
    int attractCount = 0;

    for(const Boid& b : boids) {
        if(&b == this) continue;

        float dist = getBoidDistance(*this, b);
        assert(dist != 0);

        if (dist < repelDist) {
            repel += (position - b.position);
            repel /= dist;
        }

        if (dist < attractDist) {
            attractCount++;
            attract += b.position;
        }

        if (dist < alignDist) {
            align += (b.velocity / dist);
        }
    }

    if(repel.magnitude() > 0) {
        repel.setMag(1.2);
    }

    if(attractCount) {
        attract /= attractCount; // get average position
        attract -= position;     // desired vector
        attract.setMag(0.5);
    }

    // reduce force
    if(align.magnitude() > 0) {
        align.setMag(5);
    }

    Vector2d desiredVector = align + attract + repel;
    if(desiredVector.magnitude() > 0) {
        steer = desiredVector - velocity;
        if(isinf(steer.x)) {
            std::cout << "\n";
        }
    }
    move(steer);
}

void Boid::move(const Vector2d& acceleration) {
    velocity += acceleration;
    velocity.setMag(this->speed);

    position += velocity;

    position.x = fmod(position.x, 1920);
    position.y = fmod(position.y, 1080);
    if(position.x < 0) position.x += 1920;
    if(position.y < 0) position.y += 1080;
}

Boid::Boid(Vector2d p) {
    this->mass = 1;
    this->position = p;
    this->alignDist = 140;
    this->attractDist = 120;
    this->speed = 10;
    this->repelDist = 60;
    int maxV = 5;
    int minV = -5;
    float horiz_speed = std::rand() % (maxV + 1 - minV) + minV;
    float vert_speed {0};
    do {
        vert_speed = std::rand() % (maxV + 1 - minV) + minV;
    } while (vert_speed == 0);

    this->velocity = Vector2d(horiz_speed, vert_speed);
    this->velocity.setMag(speed);
}
