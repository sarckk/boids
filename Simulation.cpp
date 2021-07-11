//
// Created by Yong Hoon Shin on 06/07/2021.
//

#include <iostream>
#include <cmath>

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

Simulation::Simulation()
        : window{nullptr}
        , startBoidCount{200}
        , margin{200} {
    initWindow();
    initImGui();
    initBoids();
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


void Simulation::updateBoids(UpdateBoidVelocityParams params, bool showTrail, sf::Time elapsed) {
    for(Boid& b: boids) {
        b.setShowTrail(showTrail);
        b.updateVelocity(boids, params);
        b.moveBounded(window->getSize(), margin, elapsed);
    }
}

void Simulation::update() {
    sf::Time elapsed = deltaClock.restart();

    pollEvents();
    updateMousePosition();
    updateImGui(elapsed);
}

void Simulation::renderBoids()
{
    for (const Boid& b : boids) {
        window->draw(b);
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
    videoMode = sf::VideoMode(1920, 1080, 32);
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

void Simulation::initBoids() {
    for(int i = 0; i < startBoidCount; i++) {
        // spawn a boid
        sf::Vector2f pos {
            (float)(rand() % window->getSize().x),
            (float)(rand() % window->getSize().y),
        };

        int maxV = 10;
        int minV = -10;
        float horiz_speed = std::rand() % (maxV + 1 - minV) + minV;
        float vert_speed;
        do {
            vert_speed = std::rand() % (maxV + 1 - minV) + minV;
        } while (vert_speed == 0);

        sf::Vector2f v {horiz_speed, vert_speed};

        boids.emplace_back(pos, v);
    }
}

void Simulation::updateMousePosition() {
    mousePosWindow = sf::Mouse().getPosition(*this->window);
}

void Simulation::updateImGui(sf::Time elapsed) {
    static int alignDist = 130;
    static float alignWeight = 0.160;
    static int cohesionDist = 150;
    static float cohesionWeight = 0.015;
    static int repelDist = 30;
    static float repelWeight = 0.045;
    static int maxSpeed = 13;
    static bool showTrail = false;
    static int mouseModifier = MouseModifier::None;
    static int mouseEffectDist = 300;

    ImGui::SFML::Update(*window, elapsed);
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + (1920 - 400), main_viewport->WorkPos.y),
                            ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(400, 1080), ImGuiCond_Once);

    ImGui::Begin("Boids Controls");

    // ID of ##On hides label
    ImGui::PushItemWidth(ImGui::GetWindowWidth());

    ImGui::Text("Align Radius");
    ImGui::SameLine(); HelpMarker("Each boid will try to align to the average velocity of all boids within adius.");
    ImGui::SliderInt("##On", &alignDist, 0, 300 );

    ImGui::Text("Align Weight");
    ImGui::SameLine(); HelpMarker("Decides how abruptly to adjust boid velocity to align with flock. "
                                  "Ranges from 0 (disable alignment) to 1 (adjust fully).");
    ImGui::SliderFloat("##1n", &alignWeight, 0, 1 );

    ImGui::Text("Attraction Radius");
    ImGui::SameLine(); HelpMarker("Each boid will be attracted to the center of mass of all boids within adius.");
    ImGui::SliderInt("##2n", &cohesionDist, 0, 300 );

    ImGui::Text("Attraction Weight");
    ImGui::SameLine(); HelpMarker("Decides how abruptly to adjust boid velocity to steer towards local center of mass. "
                                  "Ranges from 0 (disable attraction) to 1 (adjust fully).");
    ImGui::SliderFloat("##3n", &cohesionWeight, 0, 1 );

    ImGui::Text("Repulsion Radius");
    ImGui::SameLine(); HelpMarker("Each boid will experience a repulsive force from all boids within adius of itself.");
    ImGui::SliderInt("##4n", &repelDist, 0, 300 );

    ImGui::Text("Repulsion Weight");
    ImGui::SameLine(); HelpMarker("Decides how abruptly to adjust boid velocity to steer away from local boids. "
                                  "Ranges from 0 (disable repulsion) to 1 (adjust fully).");
    ImGui::SliderFloat("##5n", &repelWeight, 0, 1 );

    ImGui::Text("Flock speed");
    ImGui::SameLine(); HelpMarker("Max speed at which a boid can travel at.");
    ImGui::SliderInt("##6n", &maxSpeed, 1, 50 );

    ImGui::PopItemWidth();

    ImGui::Checkbox("Show Trail?", &showTrail);

    ImGui::Text("Mouse modifier");
    ImGui::RadioButton("None", &mouseModifier, MouseModifier::None); ImGui::SameLine();
    ImGui::RadioButton("Avoid", &mouseModifier, MouseModifier::Avoid); ImGui::SameLine();
    ImGui::RadioButton("Attract", &mouseModifier, MouseModifier::Attract);
    if(mouseModifier != MouseModifier::None) {
        ImGui::Text("Mouse effect distance");
        ImGui::SliderInt("##7n", &mouseEffectDist, 100, 1000 );
    }

    ImGui::End();

    UpdateBoidVelocityParams params;
    params.alignDist = alignDist;
    params.alignWeight = alignWeight;
    params.cohesionDist = cohesionDist;
    params.cohesionWeight = cohesionWeight;
    params.repelDist = repelDist;
    params.repelWeight = repelWeight;
    params.maxSpeed = maxSpeed;

    params.mouseEffectDist = mouseEffectDist;
    params.mouseModifier = static_cast<MouseModifier>(mouseModifier);
    params.mousePos = window->mapPixelToCoords(mousePosWindow);

    updateBoids(params, showTrail, elapsed);
}



