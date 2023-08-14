#ifndef _GRAVITYATTRACTOR_
#define _GRAVITYATTRACTOR_

#include "vec2.hpp"

struct Attractor
{
    Vec2 position;
    double mass;

    Attractor() {}

    Attractor(Vec2 initialPosition, double initialMass)
      : position(initialPosition), mass(initialMass)
    {}

    double distanceFromPoint(Vec2 point)
    {
        return subtractVec2(position, point).magnitude();
    }
    double distanceFromPoint(Vec2 *point)
    {
        return subtractVec2(position, point).magnitude();
    }
};

#endif
