#ifndef _GRAVITYVEC2_
#define _GRAVITYVEC2_

#include <cmath>

struct Vec2
{
    double x;
    double y;

    Vec2()
    {
        x = 0.0;
        y = 0.0;
    }
    Vec2(double initialX, double initialY)
        : x(initialX), y(initialY)
    {
    }

    void zero()
    {
        x = 0.0;
        y = 0.0;
    }

    void set(double newX, double newY)
    {
        x = newX;
        y = newY;
    }
    void set(Vec2 other)
    {
        x = other.x;
        y = other.y;
    }
    void set(Vec2 *other)
    {
        x = other->x;
        y = other->y;
    }

    void add(Vec2 other)
    {
        x += other.x;
        y += other.y;
    }
    void add(Vec2 *other)
    {
        x += other->x;
        y += other->y;
    }
    Vec2 addInplace(Vec2 *other) { return Vec2(x + other->x, y + other->y); }

    void subtract(Vec2 other)
    {
        x -= other.x;
        y -= other.y;
    }
    void subtract(Vec2 *other)
    {
        x -= other->x;
        y -= other->y;
    }
    Vec2 subtractInplace(Vec2 *other) { return Vec2(x - other->x, y - other->y); }

    void scale(double scalar)
    {
        x *= scalar;
        y *= scalar;
    }
    Vec2 scaleInplace(double scalar) { return Vec2(x * scalar, y * scalar); }

    double magnitude()
    {
        return std::sqrt(x * x + y * y);
    }
};

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

#endif
