//
// Created by Yong Hoon Shin on 06/07/2021.
//

#ifndef BOIDS_BOID_H
#define BOIDS_BOID_H

#define MAX_TRAIL_COUNT 40
#define AVERAGE_TIME_ELAPSED_SECONDS 0.02f

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "Arrow.h"

struct UpdateBoidPositionParams {
    int alignDist {};
    float alignWeight {};
    int attractDist {};
    float attractWeight {};
    int repelDist {};
    float repelWeight {};
    int maxSpeed {};
};

class Boid : public Arrow {
private:
    struct PositionHistory {
        sf::Time lifetime;

        PositionHistory(const sf::Time& time): lifetime(time)
        {}
    };

    sf::Vector2f velocity;
    int speed;
    float mass;
    int angle;
    bool m_showTrail;
    sf::Time m_trailLifetime;
    std::vector<sf::Vertex> m_trailVertices;
    std::vector<PositionHistory> m_positionHistory;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    explicit Boid(sf::Vector2f position, sf::Vector2f velocity);

    void updateVelocity(const std::vector<Boid>& boids, UpdateBoidPositionParams params);
    void moveBounded(unsigned int windowWidth, unsigned int windowHeight, unsigned int margin, sf::Time elapsed);

    void setShowTrail(bool showTrail);
    sf::Vector2f getVelocity() const;
};


#endif //BOIDS_BOID_H
