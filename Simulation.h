#ifndef BOIDS_SIMULATION_H
#define BOIDS_SIMULATION_H


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "SpatialHashGrid.h"
#include "Boid.h"


class Simulation {
private:
    constexpr static int SCREEN_MARGIN = 200;

    constexpr static int START_BOID_COUNT = 2;
    constexpr static int MAX_BOID_COUNT = 600;

    constexpr static int MIN_INIT_VELOCITY_XY = -10;
    constexpr static int MAX_INIT_VELOCITY_XY = 10;

    constexpr static int DEFAULT_BOID_HEIGHT = 12;
    constexpr static int DEFAULT_BOID_WIDTH = 21;

    constexpr static int MIN_BOID_HEIGHT = 7;
    constexpr static int MAX_BOID_HEIGHT = 25;

    // windows
    constexpr static int WIN_WIDTH = 1920;
    constexpr static int WIN_HEIGHT = 1080;
    constexpr static int IMGUI_WIDTH = 450;

    static const sf::Color DARK_BOID_COLOR;
    static const sf::Color LIGHT_BOID_COLOR;
    static const sf::Color PREDATOR_COLOR;

private:
    // SFML stuff
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev{};
    sf::Clock deltaClock;
    sf::ContextSettings settings;
    sf::Vector2f mousePosWindow;

    // Boids
    SpatialHashGrid m_grid;
    std::vector<std::shared_ptr<Boid>> m_boids;
    unsigned int m_margin;
    int m_boidCount;
    int m_mouseEffectDist;
    int m_mouseModifier;
    bool m_showTrail;
    bool m_randomizeSize;

    void initWindow();
    void initImGui();

    void addBoids(int count, bool randomizeSize);
    void updateBoidCount(int newCount, bool randomizeSize);
    void updateBoids(UpdateBoidVelocityParams params, bool showTrail);

    void updateMousePosition();
    UpdateBoidVelocityParams updateImGui(sf::Time elapsed);
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
