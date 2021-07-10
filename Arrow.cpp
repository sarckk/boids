//
// Created by Yong Hoon Shin on 09/07/2021.
//

#include <cmath>
#include "Arrow.h"
#include "VectorArithmetic.h"

Arrow::Arrow(sf::Vector2f position, sf::Vector2f direction, sf::Color color, int height, int width)
: m_arrow()
, m_direction(direction)
, m_color(color)
, m_height(height)
, m_width(width)
{
    setPosition(position);

    m_arrow.setFillColor(m_color);

    setDirection(m_direction);
    m_arrow.setPointCount(4);
    m_arrow.setPoint(0, sf::Vector2f(m_width, 0.f));
    m_arrow.setPoint(1, sf::Vector2f(0, m_height));
    m_arrow.setPoint(2, sf::Vector2f(m_width/3,0));
    m_arrow.setPoint(3, sf::Vector2f(0,-m_height));
}

void Arrow::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(m_arrow, states);
}

void Arrow::setDirection(sf::Vector2f direction) {
    VectorArithmetic::normalize(direction);
    float polarAngle = atan2(direction.y, direction.x) * 180 / M_PI;
    m_arrow.setRotation(polarAngle);
}

