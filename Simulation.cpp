//
// Created by Yong Hoon Shin on 06/07/2021.
//

#include <iostream>
#include <math.h>
#include "Simulation.h"
#include "Boid.h"

void Simulation::pollEvents() {
    while(this->window->pollEvent(this->ev)) {
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


void Simulation::updateBoids() {
    for(Boid& b: this->boids) {
        b.updatePosition(this->boids);
    }
}


void Simulation::update() {
    this->pollEvents();
    this->updateBoids();
}

void Simulation::renderBoids() {
    for (const Boid& b : this->boids) {
        sf::RectangleShape r { sf::Vector2f(10.f,10.f) };
        r.setPosition(b.getPosition().x, b.getPosition().y);
        r.setFillColor(sf::Color::Red);
        window->draw(r);
    }
}

void Simulation::render() {
    this->window->clear();

    this->renderBoids();

    this->window->display();
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


