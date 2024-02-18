#include "attractor.hpp"

#include "vec2.hpp"

Attractor::Attractor() {}

Attractor::Attractor(Vec2 initialPosition, double initialMass)
    : position(initialPosition), mass(initialMass)
{}

double Attractor::distanceFromPoint(Vec2 point)
{
    return subtractVec2(position, point).magnitude();
}
double Attractor::distanceFromPoint(Vec2 *point)
{
    return subtractVec2(position, point).magnitude();
}
