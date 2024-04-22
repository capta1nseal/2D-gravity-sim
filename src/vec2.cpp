#include "vec2.hpp"

#include <cmath>

Vec2::Vec2()
{
    x = 0.0;
    y = 0.0;
}
Vec2::Vec2(double initialX, double initialY)
    : x(initialX), y(initialY)
{}

void Vec2::zero()
{
    x = 0.0;
    y = 0.0;
}

void Vec2::set(double newX, double newY)
{
    x = newX;
    y = newY;
}
void Vec2::set(Vec2 other)
{
    x = other.x;
    y = other.y;
}
void Vec2::set(Vec2 *other)
{
    x = other->x;
    y = other->y;
}

Vec2 Vec2::towardsInplace(Vec2 other, double scalar)
{
    return *this + (other - *this) * scalar;
}

Vec2 Vec2::normaliseInplace(double scalar)
{
    double initialMagnitude = magnitude();

    if (initialMagnitude == 0 or initialMagnitude != initialMagnitude) { return Vec2(); } // avoid division by zero and NaN

    return *this * (scalar / initialMagnitude);
}

double Vec2::magnitude()
{
    return std::sqrt(x * x + y * y);
}

bool Vec2::operator==(Vec2 const& other)
{
    return x == other.x and y == other.y;
}

Vec2 Vec2::operator*(double const& scalar)
{
    return Vec2(x * scalar, y * scalar);
}
Vec2 Vec2::operator/(double const& scalar)
{
    return Vec2(x / scalar, y / scalar);
}

Vec2 Vec2::operator+(Vec2 const& other)
{
    return Vec2(x + other.x, y + other.y);
}
Vec2 Vec2::operator-(Vec2 const& other)
{
    return Vec2(x - other.x, y - other.y);
}
double Vec2::operator*(Vec2 const& other)
{
    return x * other.x + y * other.y;
}
double Vec2::operator%(Vec2 const& other)
{
    return x * other.y - y * other.x;
}

void Vec2::operator+=(Vec2 const& other)
{
    x += other.x;
    y += other.y;
}
