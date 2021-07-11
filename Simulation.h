#ifndef BOIDS_SIMULATION_H
#define BOIDS_SIMULATION_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "Boid.h"

class Simulation {
private:
    // SFML stuff
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev{};
    sf::Clock deltaClock;
    sf::ContextSettings settings;
    sf::Vector2i mousePosWindow;

    // Boids
    std::vector<Boid> boids;
    unsigned int startBoidCount;
    unsigned int margin;

    void initWindow();
    void initImGui();
    void initBoids();

    void updateBoids(UpdateBoidVelocityParams params, bool showTrail, sf::Time elapsed);
    void updateMousePosition();
    void updateImGui(sf::Time elapsed);
    void renderBoids();

public:
    Simulation();
    virtual ~Simulation();

    bool running() const;

    void pollEvents();
    void update();
    void render();
};


#endif //BOIDS_SIMULATION_H
