#include <iostream>
#include "Boid.h"
#include "VectorArithmetic.h"

using namespace VectorArithmetic;

Boid::Boid(const sf::Vector2f& pos, const sf::Vector2f& v, const sf::Color& c,
            float height, float width, const sf::Vector2u& windowSize, unsigned int margin, float mass, bool showNeighbors)
: Arrow(pos, v, c, height, width)
, m_velocity(v)
, m_mass(mass)
, m_showTrail(false)
, m_windowSize(windowSize)
, m_margin(margin)
, p_showNeighbors(showNeighbors)
, p_spatialIndex(-1)
{
    m_trailVertices.reserve(MAX_TRAIL_COUNT);
}

const sf::Vector2f& Boid::getVelocity() const { return m_velocity; }
const sf::Vector2f &Boid::getDirection() const { return m_direction; }
float Boid::getWidth() const { return m_width; }

sf::Vector2f Boid::alignment(const std::vector<std::shared_ptr<Boid>>& boids, float maxSpeed){
    if(boids.empty()) return {};

    sf::Vector2f desiredV;
    for(const auto b : boids) {
        desiredV += b->m_velocity;
    }
    desiredV /= (float) boids.size();
    setMag_(desiredV, maxSpeed);
    return desiredV;
}

sf::Vector2f Boid::separation(const std::vector<std::shared_ptr<Boid>>& boids, float maxSpeed) {
    if(boids.empty()) return {};

    sf::Vector2f desiredV;
    for(const auto b : boids) {
        float dist = getDistance(getPosition(), b->getPosition());
        if(dist < 1e-2) continue;
        desiredV += (getPosition() - b->getPosition()) / (dist * dist);
    }
    desiredV /= (float) boids.size();
    setMag_(desiredV, maxSpeed);
    return desiredV;
}

sf::Vector2f Boid::cohesion(const std::vector<std::shared_ptr<Boid>>& boids, float maxSpeed) {
    if(boids.empty()) return {};

    sf::Vector2f averagePosition, desiredV;
    int neighborCount = 0;

    for(const auto b : boids) {
        neighborCount++;
        averagePosition += b->getPosition();
    }
    averagePosition /= (float) neighborCount;
    desiredV = averagePosition - getPosition();
    setMag_(desiredV, maxSpeed);
    return desiredV;
}

sf::Vector2f Boid::mouseEffect(MouseModifier mouseModifier, sf::Vector2f mousePos, int mouseEffectDist, float maxSpeed) {
    sf::Vector2f desiredV;
    if(mouseModifier != MouseModifier::None) {
        if(mouseModifier == MouseModifier::Attract) desiredV = mousePos - getPosition();
        else if(mouseModifier == MouseModifier::Avoid) desiredV = getPosition() - mousePos;
        if(magnitude(desiredV) < mouseEffectDist) {
            setMag_(desiredV, maxSpeed);
            return desiredV;
        }
    }
    return {};
}

void Boid::updateVelocity(const std::vector<std::shared_ptr<Boid>>& boidsInPerceptionRadius,
                          const std::vector<std::shared_ptr<Boid>>& boidsInSeparationRadius,
                          UpdateBoidVelocityParams params) {
    sf::Vector2f steer, alignV, separationV, cohesionV, mouseV;
    float maxSpeed = params.maxSpeed;

    alignV       = alignment(boidsInPerceptionRadius, maxSpeed);
    separationV  = separation(boidsInSeparationRadius, maxSpeed);
    cohesionV    = cohesion(boidsInPerceptionRadius,  maxSpeed);
    mouseV       = mouseEffect(params.mouseModifier, params.mousePos, params.mouseEffectDist, maxSpeed);

    if(magnitude(alignV) > 0)       steer += params.alignWeight * (alignV - m_velocity);
    if(magnitude(separationV) > 0)  steer += params.separationWeight * (separationV - m_velocity);
    if(magnitude(cohesionV) > 0)    steer += params.cohesionWeight * (cohesionV - m_velocity);
    if(magnitude(mouseV) > 0)       steer += MOUSE_EFFECT_WEIGHT * (mouseV - m_velocity);

    m_acceleration = steer / m_mass;
    m_velocity += steer;

    limit_(m_velocity, params.maxSpeed);
    setDirection(m_velocity);

    // reset before next update
    m_acceleration.x = 0;
    m_acceleration.y = 0;
}

void Boid::recordPosition(){
    // clear if boid goes from one end of screen to other
    if(getSquaredDistance(getPosition(), m_trailVertices.back().position) > 200) {
        m_trailVertices.clear();
    }

    // erase if max
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
}

void Boid::move(float boundingForce, sf::Time deltaTime) {
    recordPosition();
    Arrow::move(m_velocity * deltaTime.asSeconds());

    // bound the position
    if(getPosition().x < m_margin) {
        m_velocity.x += boundingForce;
    } else if(getPosition().x > m_windowSize.x - m_margin) {
        m_velocity.x -= boundingForce;
    }

    if(getPosition().x < 0) {
        setPosition(m_windowSize.x, getPosition().y);
    } else if(getPosition().x > m_windowSize.x) {
        setPosition(0, getPosition().y);
    }

    if(getPosition().y < 0) {
        setPosition(getPosition().x, m_windowSize.y);
    } else if(getPosition().y > m_windowSize.y) {
        setPosition(getPosition().x, 0);
    }

//    if(getPosition().y < m_margin) {
//        m_velocity.y += boundingForce;
//    } else if(getPosition().y > m_windowSize.y - m_margin) {
//        m_velocity.y -= boundingForce;
//    }
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






