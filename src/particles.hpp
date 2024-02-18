#ifndef _GRAVITYPARTICLES_
#define _GRAVITYPARTICLES_

#include <vector>
#include <cmath>

#include <SDL2/SDL_render.h>

#include "vec2.hpp"
#include "camera.hpp"
#include "attractorArray.hpp"

class Particles
{
public:
    Particles();

    void setAttractorArray(AttractorArray *attractorArrayPtr);

    void generateParticles(double linearDensity, Vec2 topLeft, Vec2 bottomRight, Vec2 initialVelocity);

    void fetchTickData();

    void tick(double delta);

    void draw(SDL_Renderer *renderer, Camera *camera);
private:
    int particleCount;
    std::vector<Vec2> positionArray;
    std::vector<Vec2> previousPositionArray;
    std::vector<Vec2> velocityArray;

    Vec2 drawPosition;
    Vec2 previousDrawPosition;

    int attractorCount;
    std::vector<Attractor *> attractorPointers;
    AttractorArray *attractorArray;
};

#endif
