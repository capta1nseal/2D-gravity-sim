#ifndef _GRAVITYSIMVEC2_
#define _GRAVITYSIMVEC2_

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

    Vec2 towardsInplace(Vec2 other, double scalar);

    Vec2 normaliseInplace(double scalar);

    double magnitude();

    bool operator==(Vec2 const& other);

    Vec2 operator*(double const& scalar);
    Vec2 operator/(double const& scalar);

    Vec2 operator+(Vec2 const& other);
    Vec2 operator-(Vec2 const& other);
    double operator*(Vec2 const& other);
    double operator%(Vec2 const& other);

    void operator+=(Vec2 const& other);
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
