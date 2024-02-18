#ifndef _GRAVITYATTRACTOR_
#define _GRAVITYATTRACTOR_

#include "vec2.hpp"

struct Attractor
{
    Vec2 position;
    double mass;

    Attractor();

    Attractor(Vec2 initialPosition, double initialMass);

    double distanceFromPoint(Vec2 point);
    double distanceFromPoint(Vec2 *point);
};

#endif
