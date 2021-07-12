//
// Created by Yong Hoon Shin on 09/07/2021.
//

#ifndef BOIDS_ARROW_H
#define BOIDS_ARROW_H

#include <SFML/Graphics.hpp>

class Arrow : public sf::Drawable, public sf::Transformable {
public:
    explicit Arrow(sf::Vector2f position, sf::Vector2f direction, sf::Color color, float height, float width);
    void setDirection(sf::Vector2f direction);
    void setColor(sf::Color color);

protected:
    sf::ConvexShape m_arrow;
    float m_height;
    float m_width;
    sf::Vector2f m_position;
    sf::Vector2f m_direction;
    sf::Color m_color;

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};


#endif //BOIDS_ARROW_H
