#ifndef _GRAVITYSIMATTRACTOR_
#define _GRAVITYSIMATTRACTOR_

#include "vec2.hpp"

struct Particle
{
    Vec2 position;
    Vec2 velocity;
    double mass;

    Particle();
    Particle(Vec2 initialPosition, double initialMass);
    Particle(Vec2 initialPosition, Vec2 initialVelocity, double initialMass);
    
    double distanceFrom(Vec2 otherPosition);
    double distanceFrom(Vec2 *otherPosition);
    double distanceFrom(Particle otherAttractor);
    double distanceFrom(Particle *otherAttractor);
};

#endif
