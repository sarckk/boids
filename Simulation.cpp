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

    static int alignDist = 130;
    static float alignWeight = 0.160;
    static int attractDist = 150;
    static float attractWeight = 0.015;
    static int repelDist = 40;
    static float repelWeight = 0.06;
    static int maxSpeed = 13;

    // ID of ##On hides label
    ImGui::PushItemWidth(ImGui::GetWindowWidth());

    ImGui::Text("Align Radius");
    ImGui::SameLine(); HelpMarker("Each boid will try to align to the average velocity of all boids within this radius.");
    ImGui::SliderInt("##On", &alignDist, 0, 300 );

    ImGui::Text("Align Weight");
    ImGui::SameLine(); HelpMarker("Decides how abruptly to adjust boid velocity to align with flock. "
                                  "Ranges from 0 (disable alignment) to 1 (adjust fully).");
    ImGui::SliderFloat("##1n", &alignWeight, 0, 1 );

    ImGui::Text("Attraction Radius");
    ImGui::SameLine(); HelpMarker("Each boid will be attracted to the center of mass of all boids within this radius.");
    ImGui::SliderInt("##2n", &attractDist, 0, 300 );

    ImGui::Text("Attraction Weight");
    ImGui::SameLine(); HelpMarker("Decides how abruptly to adjust boid velocity to steer towards local center of mass. "
                                  "Ranges from 0 (disable attraction) to 1 (adjust fully).");
    ImGui::SliderFloat("##3n", &attractWeight, 0, 1 );

    ImGui::Text("Repulsion Radius");
    ImGui::SameLine(); HelpMarker("Each boid will experience a repulsive force from all boids within this radius of itself.");
    ImGui::SliderInt("##4n", &repelDist, 0, 300 );

    ImGui::Text("Repulsion Weight");
    ImGui::SameLine(); HelpMarker("Decides how abruptly to adjust boid velocity to steer away from local boids. "
                                  "Ranges from 0 (disable repulsion) to 1 (adjust fully).");
    ImGui::SliderFloat("##5n", &repelWeight, 0, 1 );

    ImGui::Text("Flock speed");
    ImGui::SameLine(); HelpMarker("Max speed at which a boid can travel at.");
    ImGui::SliderInt("##6n", &maxSpeed, 1, 50 );

    Vector2d v;
    for(auto b : boids) {
        v += b.getVelocity();
    }
    v/= boids.size();

    ImGui::PopItemWidth();

    ImGui::End();

    UpdateBoidPositionParams params;
    params.alignDist = alignDist;
    params.alignWeight = alignWeight;
    params.attractDist = attractDist;
    params.attractWeight = attractWeight;
    params.repelDist = repelDist;
    params.repelWeight = repelWeight;
    params.maxSpeed = maxSpeed;
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
    this->startBoidCount = 600;
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


