#ifndef _GRAVITYATTRACTORARRAY_
#define _GRAVITYATTRACTORARRAY_

#include <vector>

#include "attractor.hpp"
#include "vec2.hpp"
#include "camera.hpp"

class AttractorArray
{
public:
    AttractorArray()
    {
        attractorCount = 0;
        clickRadius = 25.0;
        addMass = 50.0;
    }

    void setMousePosition(SDL_Point *mousePositionPtr)
    {
        mousePosition = mousePositionPtr;
    }

    void leftClick(Camera *camera)
    {
        Vec2 position = camera->unMapCoordinate(Vec2(mousePosition->x, mousePosition->y));

        for (int i = 0; i < attractorCount; i++)
        {
            if (attractors[i].distanceFromPoint(position) * camera->getScale() < clickRadius)
            {
                // set index of attractor to be dragged
                return;
            }
        }
        addAttractor(position);
    }

    void addAttractor(Vec2 position)
    {
        attractorCount += 1;
        attractors.reserve(attractorCount);
        attractors.push_back(Attractor(position, addMass));
    }

    int getAttractorCount()
    {
        return attractorCount;
    }

    Attractor *getAttractor(int index)
    {
        return &attractors[index];
    }
private:
    int attractorCount;
    std::vector<Attractor> attractors;

    SDL_Point *mousePosition;

    double clickRadius;

    double addMass;
};

#endif
