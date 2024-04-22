#ifndef _GRAVITYSIMATTRACTOR_
#define _GRAVITYSIMATTRACTOR_

#include "vec2.hpp"

struct Attractor
{
    Vec2 position;
    double mass;

    Attractor();
    Attractor(Vec2 initialPosition, double initialMass);
    
    double distanceFrom(Vec2 otherPosition);
    double distanceFrom(Vec2 *otherPosition);
    double distanceFrom(Attractor otherAttractor);
    double distanceFrom(Attractor *otherAttractor);
};

#endif
