//
// Created by Yong Hoon Shin on 06/07/2021.
//

#include <iostream>
#include <math.h>

#include <imgui-SFML.h>
#include <imgui.h>

#include "Simulation.h"
#include "Boid.h"

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

void Simulation::pollEvents() {
    while(this->window->pollEvent(this->ev)) {
        ImGui::SFML::ProcessEvent(this->ev);

        switch(this->ev.type) {
            case sf::Event::Closed:
                this->window->close();
                break;
            case sf::Event::KeyPressed:
                if (this->ev.key.code == sf::Keyboard::Escape)
                    this->window->close();
                break;
        }
    }
}


void Simulation::updateBoids(UpdateBoidPositionParams params) {
    for(Boid& b: this->boids) {
        b.updatePosition(this->boids, params);
    }
}


void Simulation::update() {
    this->pollEvents();

    ImGui::SFML::Update(*window, deltaClock.restart());

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    int margin = 5;
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + (1920 - 400 - margin), main_viewport->WorkPos.y),
                            ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(400, 1080), ImGuiCond_Once);

    ImGui::Begin("Boids Controls");

    static int alignDist = 170;
    static float alignMag = 4;
    static int attractDist = 220;
    static float attractMag = 1.0;
    static int repelDist = 30;
    static float repelMag = 6.1;
    static int speed = 20;

    // ID of ##On hides label
    ImGui::PushItemWidth(ImGui::GetWindowWidth());

    ImGui::Text("Align Radius");
    ImGui::SameLine(); HelpMarker("Each boid will try to align to the average velocity of all boids within this radius.");
    ImGui::SliderInt("##On", &alignDist, 0, 300 );

    ImGui::Text("Align Magnitude");
    ImGui::SameLine(); HelpMarker("The magnitude that the vector representing combined alignment forces will be set to for"
                                  " the calculation of steering force.");
    ImGui::SliderFloat("##1n", &alignMag, 0, 10 );

    ImGui::Text("Attraction Radius");
    ImGui::SameLine(); HelpMarker("Each boid will be attracted to the center of mass of all boids within this radius.");
    ImGui::SliderInt("##2n", &attractDist, 0, 300 );

    ImGui::Text("Attraction Magnitude");
    ImGui::SameLine(); HelpMarker("The magnitude that the vector representing combined attractive forces will be set to for"
                                  " the calculation of steering force.");
    ImGui::SliderFloat("##3n", &attractMag, 0, 10 );

    ImGui::Text("Repulsion Radius");
    ImGui::SameLine(); HelpMarker("Each boid will experience a repulsive force from all boids within this radius of itself.");
    ImGui::SliderInt("##4n", &repelDist, 0, 300 );

    ImGui::Text("Repulsion Magnitude");
    ImGui::SameLine(); HelpMarker("The magnitude that the vector representing combined repulsive force will be set to for "
                                  " the calculation of steering force.");
    ImGui::SliderFloat("##5n", &repelMag, 0, 10 );

    ImGui::Text("Flock speed");
    ImGui::SameLine(); HelpMarker("Speed at which each boid in the flock with travel at.");
    ImGui::SliderInt("##6n", &speed, 1, 50 );

    ImGui::PopItemWidth();

    ImGui::End();

    UpdateBoidPositionParams params;
    params.alignDist = alignDist;
    params.alignMag = alignMag;
    params.attractDist = attractDist;
    params.attractMag = attractMag;
    params.repelDist = repelDist;
    params.repelMag = repelMag;
    params.speed = speed;
    this->updateBoids(params);
}

void Simulation::renderBoids() {
    for (const Boid& b : this->boids) {
        sf::RectangleShape r { sf::Vector2f(15.f,15.f) };
        r.setPosition(b.getPosition().x, b.getPosition().y);
        r.setFillColor(sf::Color::Red);
        window->draw(r);
    }
}

void Simulation::render() {
    window->clear();
    renderBoids();
    ImGui::SFML::Render(*window);
    window->display();
}

void Simulation::initVariables() {
    this->window = nullptr;
    this->startBoidCount = 500;
}

void Simulation::initWindow() {
    this->videoMode = sf::VideoMode(1920, 1080, 32);
    this->window = new sf::RenderWindow(this->videoMode, "Boids Simulation",
                                        sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(60);

    // imgui stuff
    ImGui::SFML::Init(*window);
    this->deltaClock = sf::Clock{ };

    constexpr auto scale_factor = 2.0;
    ImGui::GetIO().FontGlobalScale = scale_factor;

}

void Simulation::initBoids() {
    for(int i = 0; i < this->startBoidCount; i++) {
        // spawn a boid
        sf::Vector2u winSize = this->window->getSize();
        unsigned int winWidth = winSize.x;
        unsigned int winHeight = winSize.y;

        Vector2d p {
            (float)(rand() % winWidth),
            (float)(rand() % winHeight),
        };

        Boid boid {p};
        this->boids.push_back(boid);
    }
}

Simulation::Simulation() {
    this->initVariables();
    this->initWindow();
    this->initBoids();
}

Simulation::~Simulation() {
    delete this->window;
}

const bool Simulation::running() const {
    return this->window->isOpen();
}


