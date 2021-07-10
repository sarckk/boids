//
// Created by Yong Hoon Shin on 06/07/2021.
//

#include <cmath>
#include <iostream>
#include "Boid.h"

sf::Vector2f Boid::getVelocity() const {
    return velocity;
};

void Boid::updateVelocity(const std::vector<Boid> &boids, UpdateBoidPositionParams params) {
    sf::Vector2f steer {};
    sf::Vector2f align, attract, repel;
    int attractCount = 0;
    int alignCount = 0;
    int repelCount = 0;

    for(const Boid& b : boids) {
        if(&b == this) continue;

        float dist = getBoidDistance(*this, b);

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
}

Boid::Boid(sf::Vector2f pos, sf::Vector2f v)
: sf::Shape()
, velocity(v)
, mass(1)
{
    setPosition(pos);
    setFillColor(sf::Color::Red);
    update();
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

std::size_t Boid::getPointCount() const
{
    return 4;
}

sf::Vector2f Boid::getPoint(std::size_t index) const
{
    switch (index)
    {
        default:
        case 0: return sf::Vector2f(0, 0);
        case 1: return sf::Vector2f(5, 0);
        case 2: return sf::Vector2f(5,5);
        case 3: return sf::Vector2f(0, 5);
    }
}


