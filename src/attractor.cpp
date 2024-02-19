#include "attractor.hpp"

#include "vec2.hpp"

Attractor::Attractor()
{
    mass = 50.0;
}
Attractor::Attractor(Vec2 initialPosition, double initialMass)
    : position(initialPosition), mass(initialMass)
{}

double Attractor::distanceFrom(Vec2 otherPosition)
{
    return subtractVec2(&position, &otherPosition).magnitude();
}
double Attractor::distanceFrom(Vec2 *otherPosition)
{
    return subtractVec2(&position, otherPosition).magnitude();
}
double Attractor::distanceFrom(Attractor otherAttractor)
{
    return distanceFrom(otherAttractor.position);
}
double Attractor::distanceFrom(Attractor *otherAttractor)
{
    return distanceFrom(otherAttractor->position);
}
