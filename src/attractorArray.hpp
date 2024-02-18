#ifndef _GRAVITYATTRACTORARRAY_
#define _GRAVITYATTRACTORARRAY_

#include <vector>

#include <SDL2/SDL_rect.h>

#include "attractor.hpp"
#include "vec2.hpp"
#include "camera.hpp"

class AttractorArray
{
public:
    AttractorArray();

    void tick();

    void setMousePosition(SDL_Point *mousePositionPtr, Vec2 *worldMousePositionPtr);

    void leftClick(Camera *camera);

    void rightClick();

    void addAttractor(Vec2 position);

    void removeAttractor(int index);

    int getAttractorCount();

    Attractor *getAttractor(int index);

private:
    int attractorCount;
    std::vector<Attractor> attractors;

    SDL_Point *mousePosition;
    Vec2 *worldMousePosition;

    double clickRadius;

    bool dragging;
    int dragIndex;

    double addMass;
};

#endif
