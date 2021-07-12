#ifndef BOIDS_SIMULATION_H
#define BOIDS_SIMULATION_H


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Boid.h"


class Simulation {
private:
    constexpr static int SCREEN_MARGIN = 200;

    constexpr static int START_BOID_COUNT = 200;

    constexpr static int MIN_INIT_VELOCITY_XY = -10;
    constexpr static int MAX_INIT_VELOCITY_XY = 10;

    constexpr static int DEFAULT_BOID_HEIGHT = 12;
    constexpr static int DEFAULT_BOID_WIDTH = 21;

    constexpr static int MIN_BOID_HEIGHT = 7;
    constexpr static int MAX_BOID_HEIGHT = 25;

    static const sf::Color DARK_BOID_COLOR;
    static const sf::Color LIGHT_BOID_COLOR;

private:
    // SFML stuff
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev{};
    sf::Clock deltaClock;
    sf::ContextSettings settings;
    sf::Vector2i mousePosWindow;

    // Boids
    std::vector<Boid> m_boids;
    unsigned int m_margin;

    void initWindow();
    void initImGui();

    void addNewBoids(int count, bool randomizeSize);

    void updateBoidCount(int newCount, bool randomizeSize);
    void updateBoids(UpdateBoidVelocityParams params, bool showTrail, sf::Time elapsed);
    void updateMousePosition();
    UpdateBoidVelocityParams updateImGui(sf::Time elapsed, bool* showTrail, int* boidCount, bool* randomizeSize);
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
