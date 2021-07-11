//
// Created by Yong Hoon Shin on 06/07/2021.
//

#ifndef BOIDS_BOID_H
#define BOIDS_BOID_H

#define MAX_TRAIL_COUNT 40
#define AVERAGE_TIME_ELAPSED_SECONDS 0.02f
#define MOUSE_ATTRACTION_FACTOR 0.1f

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "Arrow.h"

enum MouseModifier {
    None,
    Avoid,
    Attract
};

struct UpdateBoidVelocityParams {
    int alignDist {};
    float alignWeight {};
    int cohesionDist {};
    float cohesionWeight {};
    int repelDist {};
    float repelWeight {};
    int maxSpeed {};

    // mouse stuff
    int mouseEffectDist;
    MouseModifier mouseModifier {};
    sf::Vector2f mousePos {};
};

class Boid : public Arrow {
private:
    struct PositionHistory {
        sf::Time lifetime;

        explicit PositionHistory(const sf::Time& time): lifetime(time)
        {}
    };

    sf::Vector2f velocity;
    float mass;
    bool m_showTrail;
    sf::Time m_trailLifetime;
    std::vector<sf::Vertex> m_trailVertices;
    std::vector<PositionHistory> m_positionHistory;

    sf::Vector2f alignment(const std::vector<Boid> &boids, int maxSpeed, int alignDist, float weight);
    sf::Vector2f repulsion(const std::vector<Boid> &boids, int maxSpeed, int repulsionDist, float weight);
    sf::Vector2f cohesion(const std::vector<Boid> &boids, int maxSpeed, int cohesionDist, float weight);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    explicit Boid(sf::Vector2f position, sf::Vector2f velocity);

    void updateVelocity(const std::vector<Boid>& boids, UpdateBoidVelocityParams params);
    void moveBounded(sf::Vector2u windowDim, unsigned int margin, sf::Time elapsed);

    void setShowTrail(bool showTrail);
    const sf::Vector2f& getVelocity() const;
};


#endif //BOIDS_BOID_H
