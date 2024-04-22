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

    Vec2 towardsInplace(Vec2 other, double scalar);

    Vec2 normaliseInplace(double scalar = 1.0);

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

#endif
