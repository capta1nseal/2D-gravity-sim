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
        previousPositionArray.reserve(particleCount);
        velocityArray.reserve(particleCount);

        for (double x = topLeft.x; x < bottomRight.x; x += 1.0 / linearDensity)
        {
            for (double y = topLeft.y; y < bottomRight.y; y += 1.0 / linearDensity)
            {
                positionArray.push_back(Vec2(x, y));
                previousPositionArray.push_back(Vec2(x, y));
                velocityArray.push_back(initialVelocity);
            }
        }
    }

    void fetchTickData()
    {
        attractorCount = attractorArray->getAttractorCount();

        attractorPointers.resize(attractorCount);
        
        for (int i = 0; i < particleCount; i++)
        {
            previousPositionArray[i] = positionArray[i];
        }

        for (int i = 0; i < attractorCount; i++)
        {
            attractorPointers[i] = attractorArray->getAttractor(i);
        }
    }

    void tick(double delta)
    {
        double dx;
        double dy;
        double forceVectorOverDistance;

        for (Attractor *currentAttractorPointer : attractorPointers)
        {
            for (int j = 0; j < particleCount; j++)
            {
                dx = currentAttractorPointer->position.x - positionArray[j].x;
                dy = currentAttractorPointer->position.y - positionArray[j].y;
                forceVectorOverDistance = (100.0 * currentAttractorPointer->mass) / pow(Vec2(dx, dy).magnitude(), 3);
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
            previousDrawPosition.set(camera->mapCoordinate(&previousPositionArray[i]));
            if (((drawPosition.x < 0 or drawPosition.x > 1919) or (drawPosition.y < 0 or drawPosition.y > 1199)) and ((previousDrawPosition.x < 0 or previousDrawPosition.x > 1919) or (previousDrawPosition.y < 0 or previousDrawPosition.y > 1199)) or (subtractVec2(drawPosition, previousDrawPosition).magnitude() > 10000.0))
                continue;
            
            SDL_RenderDrawLineF(renderer, drawPosition.x, drawPosition.y, previousDrawPosition.x, previousDrawPosition.y);
        }
    }
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
