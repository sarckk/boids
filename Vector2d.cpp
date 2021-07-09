//
// Created by Yong Hoon Shin on 07/07/2021.
//

#include <iostream>
#include <cmath>
#include <assert.h>
#include "Vector2d.h"

Vector2d operator+ (Vector2d lhs, const Vector2d& rhs) { return lhs+=rhs; }
Vector2d operator/ (Vector2d lhs, const Vector2d& rhs) { return lhs/=rhs; }
Vector2d operator* (Vector2d lhs, const Vector2d& rhs) { return lhs*=rhs; }
Vector2d operator- (Vector2d lhs, const Vector2d& rhs) { return lhs-=rhs; }

Vector2d operator*(Vector2d lhs, float rhs) { return lhs*=rhs; }
Vector2d operator/(Vector2d lhs, float rhs) { return lhs/=rhs; }

std::ostream& operator<< (std::ostream& out, const Vector2d& v) {
    out << '(' << v.x << ',' << v.y << ')' << '\n';
    return out;
}

float Vector2d::magnitude() const {
    return sqrt(x * x + y * y);
}

void Vector2d::normalize_() {
    float m = magnitude();
    if (m > 0) {
        x /= m;
        y /= m;
    }
}

void Vector2d::limit_(float limit) {
    float m = magnitude();
    if (m > limit) {
        normalize_();
        x *= limit;
        y *= limit;
    }
}

void Vector2d::setMag(float mag) {
    normalize_();
    x *= mag;
    y *= mag;
}
