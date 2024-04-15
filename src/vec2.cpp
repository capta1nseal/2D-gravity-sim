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

void Vec2::add(Vec2 other)
{
    x += other.x;
    y += other.y;
}
void Vec2::add(Vec2 *other)
{
    x += other->x;
    y += other->y;
}
Vec2 Vec2::addInplace(Vec2 *other) { return Vec2(x + other->x, y + other->y); }

void Vec2::subtract(Vec2 other)
{
    x -= other.x;
    y -= other.y;
}
void Vec2::subtract(Vec2 *other)
{
    x -= other->x;
    y -= other->y;
}
Vec2 Vec2::subtractInplace(Vec2 *other) { return Vec2(x - other->x, y - other->y); }

void Vec2::scale(double scalar)
{
    x *= scalar;
    y *= scalar;
}
Vec2 Vec2::scaleInplace(double scalar) { return Vec2(x * scalar, y * scalar); }

Vec2 Vec2::towardsInplace(Vec2 other, double scalar)
{
    return addVec2(this, subtractVec2(other, this).scaleInplace(scalar));
}

Vec2 Vec2::normaliseInplace(double scalar)
{
    double initialMagnitude = magnitude();

    if (initialMagnitude == 0 or initialMagnitude != initialMagnitude) { return Vec2(); } // avoid division by zero and NaN

    return scaleInplace(scalar / initialMagnitude);
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


Vec2 addVec2(Vec2 first, Vec2 second) { return Vec2(first.x + second.x, first.y + second.y); }
Vec2 addVec2(Vec2 first, Vec2 *second) { return Vec2(first.x + second->x, first.y + second->y); }
Vec2 addVec2(Vec2 *first, Vec2 second) { return Vec2(first->x + second.x, first->y + second.y); }
Vec2 addVec2(Vec2 *first, Vec2 *second) { return Vec2(first->x + second->x, first->y + second->y); }

Vec2 subtractVec2(Vec2 first, Vec2 second) { return Vec2(first.x - second.x, first.y - second.y); }
Vec2 subtractVec2(Vec2 first, Vec2 *second) { return Vec2(first.x - second->x, first.y - second->y); }
Vec2 subtractVec2(Vec2 *first, Vec2 second) { return Vec2(first->x - second.x, first->y - second.y); }
Vec2 subtractVec2(Vec2 *first, Vec2 *second) { return Vec2(first->x - second->x, first->y - second->y); }

Vec2 scaleVec2(Vec2 vector, double scalar) { return Vec2(vector.x * scalar, vector.y * scalar); }
Vec2 scaleVec2(Vec2 *vector, double scalar) { return Vec2(vector->x * scalar, vector->y * scalar); }
