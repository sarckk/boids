//
// Created by Yong Hoon Shin on 06/07/2021.
//

#include <assert.h>
#include "Boid.h"
#include "VectorArithmetic.h"

using namespace VectorArithmetic;

Boid::Boid(const sf::Vector2f& pos, const sf::Vector2f& v, const sf::Color& c, float height, float width, float mass = 1)
: Arrow(pos, v, c, height, width)
, m_velocity(v)
, m_mass(mass)
, m_showTrail(false)
{
    m_trailVertices.reserve(MAX_TRAIL_COUNT);
}

const sf::Vector2f& Boid::getVelocity() const {
    return m_velocity;
}

sf::Vector2f Boid::alignment(const std::vector<Boid>& boids, float maxSpeed, int alignDist, float weight){
    sf::Vector2f desiredV, deltaV;
    int neighborCount = 0;

    for(const Boid& b : boids) {
        if(&b == this) continue;
        float dist = getDistance(getPosition(), b.getPosition());

        if (dist < alignDist) {
            neighborCount++;
            desiredV += b.m_velocity;
        }
    }

    if(neighborCount) {
        desiredV /= (float) neighborCount;
        setMag(desiredV, maxSpeed);
        deltaV = weight * (desiredV - m_velocity);
    }

    return deltaV;
}

sf::Vector2f Boid::repulsion(const std::vector<Boid>& boids, float maxSpeed, int repelDist, float weight) {
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
        deltaV = weight * (desiredV - m_velocity);
    }

    return deltaV;
}

sf::Vector2f Boid::cohesion(const std::vector<Boid>& boids, float maxSpeed, int cohesionDist, float weight) {
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
        deltaV = weight * (desiredV - m_velocity);
    }

    return deltaV;
}

void Boid::updateVelocity(const std::vector<Boid>& boids, UpdateBoidVelocityParams params) {
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
            acceleration+= MOUSE_ATTRACTION_FACTOR * (desiredV - m_velocity);
        }
    }

    steer = acceleration / m_mass;
    m_velocity += steer;
    limit(m_velocity, params.maxSpeed);
    setDirection(m_velocity);
}

void Boid::moveBounded(sf::Vector2u windowDim, unsigned int margin) {
    // record the position
    if(m_trailVertices.size() == MAX_TRAIL_COUNT) {
        m_trailVertices.erase(m_trailVertices.begin());
    }

    m_trailVertices.emplace_back(getPosition(), m_color);

    // update alphas
    for (std::size_t i = 0; i < m_trailVertices.size(); i++)
    {
        float ratio = (float) i / MAX_TRAIL_COUNT;
        m_trailVertices[i].color.a = static_cast<sf::Uint8>(ratio * 255 * 0.3);
    }

    move(m_velocity);

    int boundingForce = 1;

    if(getPosition().x < margin) {
        m_velocity.x += boundingForce;
    } else if(getPosition().x > windowDim.x - margin) {
        m_velocity.x -= boundingForce;
    }

    if(getPosition().y < margin) {
        m_velocity.y += boundingForce;
    } else if(getPosition().y > windowDim.y - margin) {
        m_velocity.y -= boundingForce;
    }
}

void Boid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    Arrow::draw(target, states);

    // now draw trails
    if(m_showTrail)
        target.draw(&m_trailVertices[0], m_trailVertices.size(), sf::LineStrip, states);
}

void Boid::setShowTrail(bool showTrail) {
    m_showTrail = showTrail;
}



