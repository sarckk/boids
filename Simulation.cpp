#include <random>
#include <iostream>
#include <chrono>

#include <imgui-SFML.h>
#include <imgui.h>

#include "Simulation.h"
#include "Boid.h"

// from imgui docs
static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

static int rand_in_range(int min, int max) {
    return std::rand() % (max + 1 - min) + min;
}

const sf::Color Simulation::LIGHT_BOID_COLOR = sf::Color{75, 113, 188 };
const sf::Color Simulation::DARK_BOID_COLOR  = sf::Color{ 144, 211, 243 };
const sf::Color Simulation::PREDATOR_COLOR   = sf::Color{ 210, 48, 48 };

Simulation::Simulation()
: window(nullptr)
, m_margin(SCREEN_MARGIN)
, m_boidCount(START_BOID_COUNT)
, m_showTrail(false)
, m_randomizeSize(false)
, m_mouseModifier(MouseModifier::None)
, m_mouseEffectDist(300)
, m_grid()
{

    initWindow();
    initImGui();

    SpatialHashGrid grid { window->getSize(), 50 };
    m_grid = std::move(grid);
    addBoids(START_BOID_COUNT, false);
}

Simulation::~Simulation() {
    delete window;
}

bool Simulation::running() const {
    return window->isOpen();
}

void Simulation::pollEvents() {
    while(window->pollEvent(ev)) {
        ImGui::SFML::ProcessEvent(ev);

        switch(ev.type) {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::KeyPressed:
                if (ev.key.code == sf::Keyboard::Escape)
                    window->close();
                break;
        }
    }
}

void Simulation::updateBoids(UpdateBoidVelocityParams params, bool showTrail) {
    // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for(auto b: m_boids) {
        b->setShowTrail(showTrail);
        auto otherBoidsInPerceptionRadius = m_grid.radiusSearch(b, params.perceptionRadius);
        m_currentBoidNeighbors = otherBoidsInPerceptionRadius;
        auto otherBoidsInSeparationRadius = m_grid.radiusSearch(b, params.separationRadius);
        b->updateVelocity(otherBoidsInPerceptionRadius ,otherBoidsInSeparationRadius ,params);
        b->move(1);
        m_grid.updateBoid(b);
    }

    // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    // std::cout << "Time taken to update all boids: " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
}

UpdateBoidVelocityParams Simulation::updateImGui(sf::Time elapsed) {
    static int perceptionRadius = 90;
    static float alignWeight = 0.222;
    static float cohesionWeight = 0.080;
    static int separationRadius = 71;
    static float separationWeight = 0.085;
    static float maxSpeed = 14;

    ImGui::SFML::Update(*window, elapsed);
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + (WIN_WIDTH - IMGUI_WIDTH), main_viewport->WorkPos.y),
                            ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(IMGUI_WIDTH, WIN_HEIGHT), ImGuiCond_Once);

    ImGui::Begin("Boids Controls");

    // ID of ##On hides label
    ImGui::PushItemWidth(ImGui::GetWindowWidth());

    ImGui::Text("Perception Radius");
    ImGui::SameLine(); HelpMarker("Each boid will try to move towards the center of mass as well as align its velocity with "
                                  "all other boids in this radius.");
    ImGui::SliderInt("##On", &perceptionRadius, 0, 300 );

    ImGui::Checkbox("Highlight neighbors in radius", &m_showNeighbors);

    ImGui::Text("Align Weight");
    ImGui::SameLine(); HelpMarker("Decides how abruptly to adjust boid velocity to align with flock. "
                                  "Ranges from 0 (disable alignment) to 1 (adjust fully).");
    ImGui::SliderFloat("##1n", &alignWeight, 0, 1 );

    ImGui::Text("Cohesion Weight");
    ImGui::SameLine(); HelpMarker("Decides how abruptly to adjust boid velocity to steer towards local center of mass. "
                                  "Ranges from 0 (disable cohesion) to 1 (adjust fully).");
    ImGui::SliderFloat("##3n", &cohesionWeight, 0, 1 );

    ImGui::Text("Separation Radius");
    ImGui::SameLine(); HelpMarker("Each boid will experience a repulsive force from all boids within this radius");
    ImGui::SliderInt("##4n", &separationRadius, 0, 300 );

    ImGui::Text("Separation Weight");
    ImGui::SameLine(); HelpMarker("Decides how abruptly to adjust boid velocity to steer away from local boids. "
                                  "Ranges from 0 (disable separation) to 1 (adjust fully).");
    ImGui::SliderFloat("##5n", &separationWeight, 0, 1 );

    ImGui::Text("Flock speed");
    ImGui::SameLine(); HelpMarker("Max speed at which a boid can travel at.");
    ImGui::SliderFloat("##6n", &maxSpeed, 1, 50 );

    ImGui::PopItemWidth();

    ImGui::Checkbox("Show Trail?", &m_showTrail);
    ImGui::Checkbox("Randomize mass?", &m_randomizeSize);

    ImGui::Text("Mouse modifier");
    ImGui::RadioButton("None", &m_mouseModifier, MouseModifier::None); ImGui::SameLine();
    ImGui::RadioButton("Avoid", &m_mouseModifier, MouseModifier::Avoid); ImGui::SameLine();
    ImGui::RadioButton("Attract", &m_mouseModifier, MouseModifier::Attract);

    if(m_mouseModifier == MouseModifier::Attract || m_mouseModifier == MouseModifier::Avoid) {
        ImGui::Text("Mouse effect distance");
        ImGui::SliderInt("##7n", &m_mouseEffectDist, 100, 500 );
    }

    ImGui::Text("Boid Count");
    if(ImGui::InputInt("##8n", &m_boidCount)) {
        if(m_boidCount < 0) m_boidCount = 0;
        if(m_boidCount > MAX_BOID_COUNT) m_boidCount = MAX_BOID_COUNT;
    }

    ImGui::End();

    UpdateBoidVelocityParams params;
    params.perceptionRadius = perceptionRadius;
    params.alignWeight = alignWeight;
    params.cohesionWeight = cohesionWeight;
    params.separationRadius = separationRadius;
    params.separationWeight = separationWeight;
    params.maxSpeed = maxSpeed;

    params.mouseEffectDist = m_mouseEffectDist;
    params.mouseModifier = static_cast<MouseModifier>(m_mouseModifier);
    params.mousePos = mousePosWindow;

    return params;
}

void Simulation::updateMousePosition() {
    mousePosWindow = window->mapPixelToCoords(sf::Mouse().getPosition(*this->window));
}

void Simulation::updateBoidCount(int newCount, bool randomizeSize){
    if(newCount < m_boids.size()) {
        for(int i = 0; i < (m_boids.size() - newCount); i++) {
            // remove it from spatial hash grid first
            m_grid.removeBoid(m_boids.back());
            // then remove it from m_boids
            m_boids.pop_back();
        }
    } else if(newCount > m_boids.size()){
        addBoids(newCount - m_boids.size(), randomizeSize);
    }
}

void Simulation::addBoids(int count, bool randomizeSize) {
    for(int i = 0; i < count; i++) {
        // position
        sf::Vector2f pos {
                (float)(rand() % window->getSize().x),
                (float)(rand() % window->getSize().y),
        };

        // velocity
        float horiz_speed = rand_in_range(MIN_INIT_VELOCITY_XY, MAX_INIT_VELOCITY_XY);
        float vert_speed;
        do {
            vert_speed = rand_in_range(MIN_INIT_VELOCITY_XY, MAX_INIT_VELOCITY_XY);
        } while (vert_speed == 0);

        sf::Vector2f v {horiz_speed, vert_speed};

        // color
        sf::Color c = m_boids.size() % 2 == 0 ? DARK_BOID_COLOR : LIGHT_BOID_COLOR;

        // height and width
        float height;
        float width;

        if(!randomizeSize) {
            height = DEFAULT_BOID_HEIGHT;
            width = DEFAULT_BOID_WIDTH;
        } else {
            height = rand_in_range(MIN_BOID_HEIGHT, MAX_BOID_HEIGHT);
            width = round(height * 1.75); // roughly to maintain dimension
        }

        // mass
        float mass = randomizeSize ? (height / DEFAULT_BOID_HEIGHT) : 1;

        // show neighbors if first boid
        bool showNeighbors = m_boids.empty();

        std::shared_ptr<Boid> bPtr = std::make_shared<Boid>( pos, v, c, height, width, window->getSize(),
                                                             m_margin, mass, showNeighbors );
        m_boids.emplace_back(bPtr); // cannot be push_back because b will go out of scope otherwise.

        // add to spatial index
        m_grid.addBoid(bPtr);
    }
}

void Simulation::update() {
    sf::Time elapsed = deltaClock.restart();
    updateMousePosition();
    pollEvents();
    UpdateBoidVelocityParams params = updateImGui(elapsed);
    updateBoidCount(m_boidCount, m_randomizeSize);
    updateBoids(params, m_showTrail);
}


void Simulation::renderBoids()
{
    for (const auto b : m_boids) {
        window->draw(*b);

        // draw neighboring boids in perception radius
        if(m_showNeighbors && b->p_showNeighbors) {
            for(auto boidPtr : m_currentBoidNeighbors) {
                sf::CircleShape shape(8);
                shape.setPosition(boidPtr->getPosition());
                shape.setFillColor(sf::Color::Red);
                shape.setOutlineColor(sf::Color::White);
                shape.setOutlineThickness(1);
                window->draw(shape);
            }
        }
    }
}

void Simulation::render() {
    window->clear();
    renderBoids();
    ImGui::SFML::Render(*window);
    window->display();
}

void Simulation::initWindow() {
    settings.antialiasingLevel = 8;
    videoMode = sf::VideoMode(WIN_WIDTH, WIN_HEIGHT, 32);
    window = new sf::RenderWindow(videoMode, "Boids Simulation",
                                        sf::Style::Titlebar | sf::Style::Close,
                                        settings);
    window->setFramerateLimit(60);
}

void Simulation::initImGui(){
    ImGui::SFML::Init(*window);
    deltaClock = sf::Clock{ };

    constexpr auto scale_factor = 2.0;
    ImGui::GetIO().FontGlobalScale = scale_factor;
}
