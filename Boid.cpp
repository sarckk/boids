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

void Boid::updatePosition(const std::vector<Boid>& boids, UpdateBoidPositionParams params) {
    Vector2d steer {};
    Vector2d align, attract, repel;
    int attractCount = 0;
    int alignCount = 0;
    int repelCount = 0;

    for(const Boid& b : boids) {
        if(&b == this) continue;

        float dist = getBoidDistance(*this, b);

        if(dist == 0) {
            continue;
        }

        if (dist < params.repelDist) {
            repelCount++;
            repel += position - b.position;
            repel /= (dist * dist); // scale by how close it is
        }

        if (dist < params.attractDist) {
            attractCount++;
            attract += b.position;
        }

        if (dist < params.alignDist) {
            align += b.velocity;
            alignCount++;
        }
    }

    if(repelCount) {
        repel /= repelCount;
        repel.setMag(params.maxSpeed);
        repel -= velocity;
        repel *= params.repelWeight;
    }

    if(attractCount) {
        attract /= attractCount; // get average position
        attract -= position;
        attract.setMag(params.maxSpeed);
        attract -= velocity;
        attract *= params.attractWeight;
    }

    if(alignCount) {
        align /= alignCount;
        align.setMag(params.maxSpeed);
        align -= velocity;
        align *= params.alignWeight;
    }

    Vector2d acceleration {};
    acceleration = align + repel + attract;
    steer = acceleration / mass;
    move(steer, params.maxSpeed);
}

void Boid::move(const Vector2d& acceleration, int speed) {
    velocity += acceleration;
    velocity.limit_(speed);
    int margin = 50;

    if(position.y < margin) {
        velocity.y += 1;
    } else if(position.y > 1080 - margin) {
        velocity.y -= 1;
    }

    if(position.x < margin) {
        velocity.x += 1;
    } else if(position.x > 1920 - margin) {
        velocity.x -= 1;
    }

    position += velocity;

}

Boid::Boid(Vector2d p) {
    this->mass = 1;
    this->position = p;
    int maxV = 10;
    int minV = -10;
    float horiz_speed = std::rand() % (maxV + 1 - minV) + minV;
    float vert_speed {0};
    do {
        vert_speed = std::rand() % (maxV + 1 - minV) + minV;
    } while (vert_speed == 0);

    this->velocity = Vector2d(horiz_speed, vert_speed);
}
