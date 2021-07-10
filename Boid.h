//
// Created by Yong Hoon Shin on 06/07/2021.
//

#ifndef BOIDS_BOID_H
#define BOIDS_BOID_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

struct UpdateBoidPositionParams {
    int alignDist {};
    float alignWeight {};
    int attractDist {};
    float attractWeight {};
    int repelDist {};
    float repelWeight {};
    int maxSpeed {};
};

class Boid : public sf::Shape {
private:
    sf::Vector2f velocity;
    int speed;
    float mass;
    int angle;

public:
    explicit Boid(sf::Vector2f position, sf::Vector2f velocity);

    void updateVelocity(const std::vector<Boid>& boids, UpdateBoidPositionParams params);

    void moveBounded(unsigned int windowWidth, unsigned int windowHeight, unsigned int margin);
    sf::Vector2f getVelocity() const;

    // sf::Shape methods
    std::size_t getPointCount() const override;
    sf::Vector2f getPoint(std::size_t index) const override;

    // static helper methods
    static float getBoidDistance(const Boid& a, const Boid& b) {
        float d_x = a.getPosition().x - b.getPosition().x;
        float d_y = a.getPosition().y - b.getPosition().y;
        return static_cast<float>(sqrt(d_x * d_x + d_y * d_y));
    }

    static float magnitude(sf::Vector2f& vec)  {
        return static_cast<float>(sqrt(vec.x * vec.x + vec.y * vec.y));
    }

    static void normalize(sf::Vector2f& vec)  {
        float m = magnitude(vec);
        if(m > 0.) {
            vec /= m;
        }
    }

    static void limit(sf::Vector2f& vec, float limit)  {
        float m = magnitude(vec);
        if(m > 0 && m > limit) {
            normalize(vec);
            vec *= limit;
        }
    }

    static void setMag(sf::Vector2f& vec, float mag)  {
        normalize(vec);
        vec *= mag;
    }
};


#endif //BOIDS_BOID_H
