#include <cmath>
#include "Arrow.h"
#include "VectorArithmetic.h"

using namespace VectorArithmetic;

Arrow::Arrow(sf::Vector2f position, sf::Vector2f direction, sf::Color color, float height, float width)
: m_arrow()
, m_direction(normalizeCopy(direction))
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
    m_direction = normalizeCopy(direction);
    float polarAngle = atan2(m_direction.y, m_direction.x) * 180 / M_PI;
    m_arrow.setRotation(polarAngle);
}

void Arrow::setColor(sf::Color color) {
    m_arrow.setFillColor(color);
}


