//
// Created by Yong Hoon Shin on 06/07/2021.
//

#ifndef BOIDS_BOID_H
#define BOIDS_BOID_H


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
    constexpr static int    MAX_TRAIL_COUNT = 90;
    constexpr static float  AVERAGE_TIME_ELAPSED_SECONDS = 0.02f;
    constexpr static float  MOUSE_ATTRACTION_FACTOR = 0.1f;
    constexpr static int    NEAR_NEIGHBOR_RADIUS = 50;

private:
    struct PositionHistory {
        sf::Time lifetime;

        explicit PositionHistory(const sf::Time& time): lifetime(time)
        {}
    };

    sf::Vector2f m_velocity;
    float m_mass;
    bool m_showTrail;
    sf::Time m_trailLifetime;
    std::vector<sf::Vertex> m_trailVertices;
    std::vector<PositionHistory> m_positionHistory;

    sf::Vector2f alignment(const std::vector<Boid> &boids, float maxSpeed, int alignDist, float weight);
    sf::Vector2f repulsion(const std::vector<Boid> &boids, float maxSpeed, int repulsionDist, float weight);
    sf::Vector2f cohesion(const std::vector<Boid> &boids, float maxSpeed, int cohesionDist, float weight);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    explicit Boid(const sf::Vector2f& position, const sf::Vector2f& velocity, const sf::Color& color,
                  float height, float width, float mass);

    void updateVelocity(const std::vector<Boid>& boids, UpdateBoidVelocityParams params);
    void moveBounded(sf::Vector2u windowDim, unsigned int margin, sf::Time elapsed);

    void setShowTrail(bool showTrail);
    const sf::Vector2f& getVelocity() const;
};


#endif //BOIDS_BOID_H
