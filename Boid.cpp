//
// Created by Yong Hoon Shin on 06/07/2021.
//

#include "Boid.h"
#include "VectorArithmetic.h"

sf::Vector2f Boid::getVelocity() const {
    return velocity;
};

void Boid::updateVelocity(const std::vector<Boid> &boids, UpdateBoidPositionParams params) {
    using namespace VectorArithmetic;
    sf::Vector2f steer {};
    sf::Vector2f align, attract, repel;
    int attractCount = 0;
    int alignCount = 0;
    int repelCount = 0;

    for(const Boid& b : boids) {
        if(&b == this) continue;

        float dist = getDistance(this->getPosition(), b.getPosition());

        if(dist < 1e-2) continue;

        if (dist < params.repelDist) {
            repelCount++;
            repel += (getPosition() - b.getPosition()) / (dist * dist);
        }

        if (dist < params.attractDist) {
            attractCount++;
            attract += b.getPosition();
        }

        if (dist < params.alignDist) {
            align += b.velocity;
            alignCount++;
        }
    }

    if(repelCount) {
        repel /= (float) repelCount;
        setMag(repel, params.maxSpeed);
        repel -= velocity;
        repel *= params.repelWeight;
    }


    if(attractCount) {
        attract /= (float) attractCount; // get average position
        attract -= getPosition();
        setMag(attract, params.maxSpeed);
        attract -= velocity;
        attract *= params.attractWeight;
    }

    if(alignCount) {
        align /= (float) alignCount;
        setMag(align, params.maxSpeed);
        align -= velocity;
        align *= params.alignWeight;
    }

    sf::Vector2f acceleration {};
    acceleration = align + repel + attract;
    steer = acceleration / mass;

    velocity += steer;
    limit(velocity, params.maxSpeed);

    setDirection(velocity);
}

Boid::Boid(sf::Vector2f pos, sf::Vector2f v)
: Arrow(pos, v, sf::Color::Red, 7, 15)
, velocity(v)
, mass(1)
{
}

void Boid::moveBounded(unsigned int windowWidth, unsigned int windowHeight, unsigned int margin) {
    move(velocity);
    int boundingForce = 1;

    if(getPosition().x < margin) {
        velocity.x += boundingForce;
    } else if(getPosition().x > windowWidth - margin) {
        velocity.x -= boundingForce;
    }

    if(getPosition().y < margin) {
        velocity.y += boundingForce;
    } else if(getPosition().y > windowHeight - margin) {
        velocity.y -= boundingForce;
    }
}


