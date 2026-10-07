#pragma once
#include <cmath>

struct Vec2 {
    double x = 0.0;
    double y = 0.0;

    constexpr Vec2& operator+=(Vec2 other) {
        x += other.x;
        y += other.y;
        return *this;   
    }

    constexpr Vec2& operator-=(Vec2 other) {
        x -= other.x;
        y -= other.y;
        return *this;   
    }

    constexpr double lengthSquared() const {
        return x * x + y * y;
    }
    double length() const {
        return std::sqrt(lengthSquared());
    }
};

constexpr Vec2 operator+(Vec2 a, Vec2 b) {
    return {a.x + b.x, a.y + b.y};
}
constexpr Vec2 operator*(Vec2 a, Vec2 b) {
    return {a.x * b.x, a.y * b.y};
}
constexpr Vec2 operator*(double s, Vec2 v) {
    return {v.x * s, v.y * s};
}
constexpr Vec2 operator/(Vec2 v, double s) {
    return {v.x / s, v.y / s};
}
constexpr Vec2 operator-(Vec2 a, Vec2 b) {
    return {a.x - b.x, a.y - b.y};
}
constexpr Vec2 operator-(Vec2 v) {
    return {-v.x, -v.y};
}