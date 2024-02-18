#include "attractorArray.hpp"

#include <vector>

#include <SDL2/SDL_rect.h>

#include "attractor.hpp"
#include "vec2.hpp"
#include "camera.hpp"

AttractorArray::AttractorArray()
{
    attractorCount = 0;
    clickRadius = 25.0;
    dragging = false;
    dragIndex = 0;
    addMass = 50.0;
}

void AttractorArray::tick()
{
    if (dragging)
    {
        attractors[dragIndex].position.set(Vec2(worldMousePosition->x, worldMousePosition->y));
    }
}

void AttractorArray::setMousePosition(SDL_Point *mousePositionPtr, Vec2 *worldMousePositionPtr)
{
    mousePosition = mousePositionPtr;
    worldMousePosition = worldMousePositionPtr;
}

void AttractorArray::leftClick(Camera *camera)
{
    if (dragging)
    {
        dragging = false;
        dragIndex = 0;
        return;
    }

    int closestAttractorIndex = -1;
    double closestAttractorDistance = clickRadius;
    double distance;

    for (int i = 0; i < attractorCount; i++)
    {
        distance = attractors[i].distanceFromPoint(worldMousePosition);
        if (distance * camera->getScale() < closestAttractorDistance)
        {
            closestAttractorIndex = i;
            closestAttractorDistance = distance;
        }
    }
    if (closestAttractorIndex >= 0)
    {
        dragIndex = closestAttractorIndex;
        dragging = true;
        return;
    }
    addAttractor(*worldMousePosition);
}

void AttractorArray::rightClick()
{
    if (attractorCount == 0)
        return;

    if (attractorCount == 1)
    {
        removeAttractor(0);
        return;
    }

    int closestAttractorIndex = 0;
    double closestAttractorDistance = INFINITY;
    double distance;
    
    for (int i = 0; i < attractorCount; i++)
    {
        distance = attractors[i].distanceFromPoint(worldMousePosition);
        if (distance < closestAttractorDistance)
        {
            closestAttractorIndex = i;
            closestAttractorDistance = distance;
        }
    }
    if (closestAttractorDistance != INFINITY)
        removeAttractor(closestAttractorIndex);
}

void AttractorArray::addAttractor(Vec2 position)
{
    attractors.reserve(attractorCount);
    attractors.push_back(Attractor(position, addMass));
    attractorCount += 1;
}

void AttractorArray::removeAttractor(int index)
{
    if (dragging)
    {
        if (dragIndex > index)
        {
            dragIndex -= 1;
        }
        else if (dragIndex == index)
        {
            dragging = false;
            dragIndex = 0;
        }
    }
    attractors.erase(attractors.begin() + index);
    attractorCount -= 1;
    attractors.resize(attractorCount);
}

int AttractorArray::getAttractorCount()
{
    return attractorCount;
}

Attractor *AttractorArray::getAttractor(int index)
{
    return &attractors[index];
}