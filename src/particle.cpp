#include "particle.hpp"

#include "vec2.hpp"

Particle::Particle()
{
    mass = 50.0;
}
Particle::Particle(Vec2 initialPosition, double initialMass)
    : position(initialPosition), mass(initialMass)
{}
Particle::Particle(Vec2 initialPosition, Vec2 initialVelocity, double initialMass)
    : position(initialPosition), velocity(initialVelocity), mass(initialMass)
{}

double Particle::distanceFrom(Vec2 otherPosition)
{
    return (position - otherPosition).magnitude();
}
double Particle::distanceFrom(Vec2 *otherPosition)
{
    return (position - *otherPosition).magnitude();
}
double Particle::distanceFrom(Particle otherAttractor)
{
    return distanceFrom(otherAttractor.position);
}
double Particle::distanceFrom(Particle *otherAttractor)
{
    return distanceFrom(otherAttractor->position);
}
