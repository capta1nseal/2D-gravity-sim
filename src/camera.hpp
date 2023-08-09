#ifndef _GRAVITYCAMERA_
#define _GRAVITYCAMERA_

#include "vec2.hpp"
#include "input.hpp"
#include "math.hpp"

class Camera
{
public:
    Camera()
    {
        scale = 0.1;
        targetScale = 1.0;
        scaleApproachQuotient = 2.5;

        motionSpeed = 500.0;
        positionApproachQuotient = 5.0;

        zoomFactor = 2.5;
        minScale = 0.1;
        maxScale = 10.0;
    }

    void initializeResolution(int initialDisplayWidth, int initialDisplayHeight)
    {
        centreVector = Vec2(static_cast<double>(initialDisplayWidth) / 2.0, static_cast<double>(initialDisplayHeight) / 2.0);
    }

    void setInput(Input *inputPtr)
    {
        input = inputPtr;
    }

    void setPosition(Vec2 newPosition)
    {
        targetPosition.set(&newPosition);
        position.set(&newPosition);
    }

    void setTargetPosition(Vec2 newTargetPosition)
    {
        targetPosition.set(newTargetPosition);
    }
    void setTargetPosition(Vec2 *newTargetPosition)
    {
        targetPosition.set(newTargetPosition);
    }

    void setScale(double newScale)
    {
        targetScale = clamp(newScale, minScale, maxScale);
        scale = targetScale;
    }

    void setTargetScale(double newTargetScale)
    {
        targetScale = clamp(newTargetScale, minScale, maxScale);
    }

    double getScale()
    {
        return scale;
    }

    Vec2 mapCoordinate(Vec2 coordinate)
    {
        return addVec2(
            &centreVector,
            scaleVec2(subtractVec2(&coordinate, &position), scale)
        );
    }
    Vec2 mapCoordinate(Vec2 *coordinate)
    {
        return addVec2(
            &centreVector,
            scaleVec2(subtractVec2(coordinate, &position), scale)
        );
    }

    Vec2 unMapCoordinate(Vec2 coordinate)
    {
        return addVec2(
            scaleVec2(subtractVec2(&coordinate, &centreVector), 1.0 / scale),
            &position
        );
    }
    Vec2 unMapCoordinate(Vec2 *coordinate)
    {
        return addVec2(
            scaleVec2(subtractVec2(coordinate, &centreVector), 1.0 / scale),
            &position
        );
    }


    void tick(double delta)
    {
        if (input->zoomInPressed())
            zoomIn(delta);
        if (input->zoomOutPressed())
            zoomOut(delta);
        
        if (input->upPressed())
            targetPosition.add(Vec2( 0.0,-1.0 * motionSpeed * delta * (1 / scale)));
        if (input->rightPressed())
            targetPosition.add(Vec2( 1.0 * motionSpeed * delta * (1 / scale), 0.0));
        if (input->downPressed())
            targetPosition.add(Vec2( 0.0, 1.0 * motionSpeed * delta * (1 / scale)));
        if (input->leftPressed())
            targetPosition.add(Vec2(-1.0 * motionSpeed * delta * (1 / scale), 0.0));

        position.add(scaleVec2(subtractVec2(&targetPosition, &position), positionApproachQuotient * delta));
        scale += (targetScale - scale) * scaleApproachQuotient * delta;
    }

private:
    Vec2 centreVector;
    Vec2 position;
    Vec2 targetPosition;
    double positionApproachQuotient;

    double scale;
    double targetScale;
    double scaleApproachQuotient;

    double motionSpeed;

    double zoomFactor;
    double minScale;
    double maxScale;

    Input *input;

    void zoomIn(double delta)
    {
        targetScale = clamp(targetScale * (1 + (zoomFactor - 1) * delta), minScale, maxScale);
    }

    void zoomOut(double delta)
    {
        targetScale = clamp(targetScale / (1 + (zoomFactor - 1) * delta), minScale, maxScale);
    }
};

#endif
