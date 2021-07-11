//
// Created by Yong Hoon Shin on 06/07/2021.
//

#include <iostream>
#include "Boid.h"
#include "VectorArithmetic.h"

using namespace VectorArithmetic;

Boid::Boid(sf::Vector2f pos, sf::Vector2f v)
: Arrow(pos, v, sf::Color::Red, 7, 15)
, velocity(v)
, mass(1)
, m_showTrail(false)
, m_trailLifetime(sf::seconds(AVERAGE_TIME_ELAPSED_SECONDS * MAX_TRAIL_COUNT))
{
    m_trailVertices.reserve(MAX_TRAIL_COUNT);
    m_positionHistory.reserve(MAX_TRAIL_COUNT);
}

const sf::Vector2f& Boid::getVelocity() const {
    return velocity;
}

sf::Vector2f Boid::alignment(const std::vector<Boid> &boids, int maxSpeed, int alignDist, float weight){
    sf::Vector2f desiredV, deltaV;
    int neighborCount = 0;

    for(const Boid& b : boids) {
        if(&b == this) continue;
        float dist = getDistance(getPosition(), b.getPosition());

        if (dist < alignDist) {
            neighborCount++;
            desiredV += b.velocity;
        }
    }

    if(neighborCount) {
        desiredV /= (float) neighborCount;
        setMag(desiredV, maxSpeed);
        deltaV = weight * (desiredV - velocity);
    }

    return deltaV;
}

sf::Vector2f Boid::repulsion(const std::vector<Boid> &boids, int maxSpeed, int repelDist, float weight) {
    sf::Vector2f desiredV, deltaV;
    int neighborCount = 0;

    for(const Boid& b : boids) {
        if(&b == this) continue;
        float dist = getDistance(getPosition(), b.getPosition());

        if(dist < 1e-2) continue;

        if (dist < repelDist) {
            neighborCount++;
            desiredV += (getPosition() - b.getPosition()) / (dist * dist);
        }
    }

    if(neighborCount) {
        desiredV /= (float) neighborCount;
        setMag(desiredV, maxSpeed);
        deltaV = weight * (desiredV - velocity);
    }

    return deltaV;
}

sf::Vector2f Boid::cohesion(const std::vector<Boid> &boids, int maxSpeed, int cohesionDist, float weight) {
    sf::Vector2f averagePosition, desiredV, deltaV;
    int neighborCount = 0;

    for(const Boid& b : boids) {
        if(&b == this) continue;
        float dist = getDistance(getPosition(), b.getPosition());

        if (dist < cohesionDist) {
            neighborCount++;
            averagePosition += b.getPosition();
        }
    }

    if(neighborCount) {
        averagePosition /= (float) neighborCount;
        desiredV = averagePosition - getPosition();
        setMag(desiredV, maxSpeed);
        deltaV = weight * (desiredV - velocity);
    }

    return deltaV;
}

void Boid::updateVelocity(const std::vector<Boid> &boids, UpdateBoidVelocityParams params) {
    sf::Vector2f steer, acceleration;
    float maxSpeed = params.maxSpeed;

    acceleration = alignment(boids, maxSpeed, params.alignDist, params.alignWeight);
    acceleration += repulsion(boids, maxSpeed, params.repelDist, params.repelWeight);
    acceleration += cohesion(boids, maxSpeed, params.cohesionDist, params.cohesionWeight);

    if(params.mouseModifier != MouseModifier::None) {
        sf::Vector2f desiredV;
        if(params.mouseModifier == MouseModifier::Attract) desiredV = params.mousePos - getPosition();
        else if(params.mouseModifier == MouseModifier::Avoid) desiredV = getPosition() - params.mousePos;
        if(magnitude(desiredV) < params.mouseEffectDist) {
            setMag(desiredV, maxSpeed);
            acceleration+= MOUSE_ATTRACTION_FACTOR * (desiredV - velocity);
        }
    }

    steer = acceleration / mass;
    velocity += steer;
    limit(velocity, params.maxSpeed);
    setDirection(velocity);
}

void Boid::moveBounded(sf::Vector2u windowDim, unsigned int margin, sf::Time elapsed) {
    // record the position
    sf::Color c {255,0,0 };
    m_trailVertices.emplace_back(getPosition(), c);
    m_positionHistory.emplace_back(m_trailLifetime);

    if(m_trailVertices.size() > MAX_TRAIL_COUNT) {
        m_trailVertices.erase(m_trailVertices.begin());
        m_positionHistory.erase(m_positionHistory.begin());
    }

    // update alphas
    for (std::size_t i = 0; i < m_positionHistory.size(); ++i)
    {
        PositionHistory& pHistory = m_positionHistory[i];
        pHistory.lifetime -= elapsed;

        float ratio = pHistory.lifetime.asSeconds() / m_trailLifetime.asSeconds();
        m_trailVertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
    }

    move(velocity);

    int boundingForce = 1;

    if(getPosition().x < margin) {
        velocity.x += boundingForce;
    } else if(getPosition().x > windowDim.x - margin) {
        velocity.x -= boundingForce;
    }

    if(getPosition().y < margin) {
        velocity.y += boundingForce;
    } else if(getPosition().y > windowDim.y - margin) {
        velocity.y -= boundingForce;
    }
}

void Boid::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    Arrow::draw(target, states);

    // now draw trails
    if(m_showTrail)
        target.draw(&m_trailVertices[0], m_trailVertices.size(), sf::LineStrip, states);
}

void Boid::setShowTrail(bool showTrail) {
    m_showTrail = showTrail;
}

