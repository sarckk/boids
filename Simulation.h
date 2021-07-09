#ifndef BOIDS_SIMULATION_H
#define BOIDS_SIMULATION_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "Boid.h"

class Simulation {
private:
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev;
    sf::Clock deltaClock;

    // Boids
    std::vector<Boid> boids;
    unsigned startBoidCount;

    void initVariables();
    void initWindow();
    void initBoids();

    void updateBoids(UpdateBoidPositionParams params);
    void renderBoids();

public:
    Simulation();
    virtual ~Simulation();

    const bool running() const;

    void pollEvents();
    void update();
    void render();
};


#endif //BOIDS_SIMULATION_H
