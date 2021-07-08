//
// Created by Yong Hoon Shin on 07/07/2021.
//

#ifndef BOIDS_VECTOR2D_H
#define BOIDS_VECTOR2D_H

#include <SFML/System.hpp>

class Vector2d {
public:
    float x {0};
    float y {0};

    Vector2d() = default;
    Vector2d(float x, float y): x{x}, y{y} {};
    Vector2d(const Vector2d& other): x{other.x}, y{other.y} {};
    explicit Vector2d(const sf::Vector2f sfVec): x{sfVec.x}, y{sfVec.y} {};

    Vector2d& operator*=(float a) {x *= a; y *= a; return *this;}
    Vector2d& operator/=(float a) {x /= a; y /= a; return *this;}

    Vector2d& operator-=(const Vector2d& other) {x -= other.x; y -= other.y; return *this;}
    Vector2d& operator*=(const Vector2d& other) {x *= other.x; y *= other.y; return *this;}
    Vector2d& operator/=(const Vector2d& other) {x /= other.x; y /= other.y; return *this;}
    Vector2d& operator+=(const Vector2d& other) {x += other.x; y += other.y; return *this;}

    bool operator==(const Vector2d& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Vector2d& other) const { return !(*this == other); }

    // conversion to and from sf::Vector2f
    explicit operator sf::Vector2f() const {return sf::Vector2f(x, y); }

    float magnitude() const;
    void normalize_();
    void limit_(float limit);
    void setMag(float mag);
};

Vector2d operator+(const Vector2d lhs, const Vector2d& rhs);
Vector2d operator-(const Vector2d lhs, const Vector2d& rhs);
Vector2d operator*(const Vector2d lhs, const Vector2d& rhs);
Vector2d operator/(const Vector2d lhs, const Vector2d& rhs);
Vector2d operator*(Vector2d lhs, float rhs);
Vector2d operator/(Vector2d lhs, float rhs);
std::ostream& operator<< (std::ostream& out, const Vector2d& v);

#endif //BOIDS_VECTOR2D_H
