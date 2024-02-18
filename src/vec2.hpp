#ifndef _GRAVITYVEC2_
#define _GRAVITYVEC2_

#include <cmath>

struct Vec2
{
    double x;
    double y;

    Vec2();
    Vec2(double initialX, double initialY);

    void zero();

    void set(double newX, double newY);
    void set(Vec2 other);
    void set(Vec2 *other);

    void add(Vec2 other);
    void add(Vec2 *other);
    Vec2 addInplace(Vec2 *other);

    void subtract(Vec2 other);
    void subtract(Vec2 *other);
    Vec2 subtractInplace(Vec2 *other);

    void scale(double scalar);
    Vec2 scaleInplace(double scalar);

    double magnitude();
};

Vec2 addVec2(Vec2 first, Vec2 second);
Vec2 addVec2(Vec2 first, Vec2 *second);
Vec2 addVec2(Vec2 *first, Vec2 second);
Vec2 addVec2(Vec2 *first, Vec2 *second);

Vec2 subtractVec2(Vec2 first, Vec2 second);
Vec2 subtractVec2(Vec2 first, Vec2 *second);
Vec2 subtractVec2(Vec2 *first, Vec2 second);
Vec2 subtractVec2(Vec2 *first, Vec2 *second);

Vec2 scaleVec2(Vec2 vector, double scalar);
Vec2 scaleVec2(Vec2 *vector, double scalar);

#endif
