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
    Particles()
    {
    }

    void setAttractorArray(AttractorArray *attractorArrayPtr)
    {
        attractorArray = attractorArrayPtr;
    }

    void generateParticles(double linearDensity, Vec2 topLeft, Vec2 bottomRight, Vec2 initialVelocity)
    {
        particleCount = 0;
        for (double x = topLeft.x; x < bottomRight.x; x += 1.0 / linearDensity)
        {
            for (double y = topLeft.y; y < bottomRight.y; y += 1.0 / linearDensity)
            {
                particleCount += 1;
            }
        }

        positionArray.reserve(particleCount);
        velocityArray.reserve(particleCount);

        for (double x = topLeft.x; x < bottomRight.x; x += 1.0 / linearDensity)
        {
            for (double y = topLeft.y; y < bottomRight.y; y += 1.0 / linearDensity)
            {
                positionArray.push_back(Vec2(x, y));
                velocityArray.push_back(initialVelocity);
            }
        }
    }

    void tick(double delta)
    {
        int attractorCount = attractorArray->getAttractorCount();

        double dx;
        double dy;
        double forceVectorOverDistance;
        Attractor attractor;

        for (int i = 0; i < attractorCount; i++)
        {
            attractor = attractorArray->getAttractor(i);
            for (int j = 0; j < particleCount; j++)
            {
                dx = attractor.position.x - positionArray[j].x;
                dy = attractor.position.y - positionArray[j].y;
                forceVectorOverDistance = 5000 / pow(Vec2(dx, dy).magnitude(), 3);
                velocityArray[j].add(Vec2(forceVectorOverDistance * dx, forceVectorOverDistance * dy));
            }
        }
        for (int i = 0; i < particleCount; i++)
        {
            positionArray[i].add(velocityArray[i].scaleInplace(delta));
        }
    }

    void draw(SDL_Renderer *renderer, Camera *camera)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < particleCount; i++)
        {
            drawPosition.set(camera->mapCoordinate(&positionArray[i]));

            SDL_RenderDrawPointF(renderer, drawPosition.x, drawPosition.y);
        }
    }
private:
    int particleCount;
    std::vector<Vec2> positionArray;
    std::vector<Vec2> velocityArray;

    Vec2 drawPosition;

    AttractorArray *attractorArray;
};

#endif
