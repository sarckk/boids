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
    Attract,
};

struct UpdateBoidVelocityParams {
    int perceptionRadius {};
    float alignWeight {};
    float cohesionWeight {};
    int separationRadius {};
    float separationWeight {};
    float maxSpeed {};

    // mouse stuff
    int mouseEffectDist;
    MouseModifier mouseModifier {};
    sf::Vector2f mousePos {};
};

class Boid : public Arrow {
private:
    constexpr static int    MAX_TRAIL_COUNT = 70;
    constexpr static float  MOUSE_EFFECT_WEIGHT = 0.1f;
    constexpr static int    NEAR_NEIGHBOR_RADIUS = 50;

private:
    float m_mass;
    bool m_showTrail;
    bool m_isPredator;
    sf::Vector2f m_velocity;
    sf::Vector2f m_acceleration;

    sf::Vector2i m_windowSize;
    int m_margin;

    // drawing stuff
    std::vector<sf::Vertex> m_trailVertices;

    sf::Vector2f alignment(const std::vector<std::shared_ptr<Boid>>& boids, float maxSpeed);
    sf::Vector2f separation(const std::vector<std::shared_ptr<Boid>>& boids, float maxSpeed);
    sf::Vector2f cohesion(const std::vector<std::shared_ptr<Boid>>& boids, float maxSpeed);
    sf::Vector2f mouseEffect(MouseModifier mouseModifier, sf::Vector2f mousePos, int mouseEffectDist, float maxSpeed);

    void recordPosition();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    int p_spatialIndex;
    bool p_showNeighbors;

    explicit Boid(const sf::Vector2f& position, const sf::Vector2f& velocity, const sf::Color& color,
                  float height, float width, const sf::Vector2u& windowSize, unsigned int margin, float mass, bool showNeighbors);

    void updateVelocity(const std::vector<std::shared_ptr<Boid>>& boidInPerceptionRadius,
                        const std::vector<std::shared_ptr<Boid>>& boidsInSeparationRadius,
                        UpdateBoidVelocityParams params);
    void move(float boundingForce, sf::Time deltaTime);

    void setShowTrail(bool showTrail);

    const sf::Vector2f& getVelocity() const;
    const sf::Vector2f& getDirection() const;
    float getWidth() const;
};


#endif //BOIDS_BOID_H
