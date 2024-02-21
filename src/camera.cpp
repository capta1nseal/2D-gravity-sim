#include "camera.hpp"

#include "vec2.hpp"
#include "input.hpp"
#include "math.hpp"


Camera::Camera()
{
    scale = 0.00001;
    previousScale = 0.00001;
    targetScale = 0.01;
    scaleApproachQuotient = 2.5;

    motionSpeed = 1.0;
    positionApproachQuotient = 5.0;

    zoomFactor = 2.5;
    minScale = 0.00001;
    maxScale = 10.0;
}

void Camera::initializeResolution(int initialDisplayWidth, int initialDisplayHeight)
{
    centreVector = Vec2(static_cast<double>(initialDisplayWidth) / 2.0, static_cast<double>(initialDisplayHeight) / 2.0);
}

void Camera::setInput(Input *inputPtr)
{
    input = inputPtr;
}

void Camera::setPosition(Vec2 newPosition)
{
    targetPosition.set(&newPosition);
    previousPosition.set(&newPosition);
    position.set(&newPosition);
}

void Camera::setTargetPosition(Vec2 newTargetPosition)
{
    targetPosition.set(newTargetPosition);
}

void Camera::setTargetPosition(Vec2 *newTargetPosition)
{
    targetPosition.set(newTargetPosition);
}

void Camera::setScale(double newScale)
{
    targetScale = clamp(newScale, minScale, maxScale);
    scale = targetScale;
    previousScale = targetScale;
}

void Camera::setTargetScale(double newTargetScale)
{
    targetScale = clamp(newTargetScale, minScale, maxScale);
}

double Camera::getScale()
{
    return scale;
}

Vec2 Camera::mapCoordinate(Vec2 coordinate)
{
    return mapCoordinate(&coordinate);
}
Vec2 Camera::mapCoordinate(Vec2 *coordinate)
{
    Vec2 mappedCoordinate = scaleVec2(subtractVec2(coordinate, &position), scale);
    mappedCoordinate.y *= centreVector.x / centreVector.y;
    return mappedCoordinate;
}

Vec2 Camera::mapPreviousCoordinate(Vec2 coordinate)
{
    return mapPreviousCoordinate(&coordinate);
}
Vec2 Camera::mapPreviousCoordinate(Vec2 *coordinate)
{
    Vec2 mappedCoordinate = scaleVec2(subtractVec2(coordinate, &previousPosition), previousScale);
    mappedCoordinate.y *= centreVector.x / centreVector.y;
    return mappedCoordinate;
}

Vec2 Camera::unMapCoordinate(Vec2 coordinate)
{
    return addVec2(
        scaleVec2(subtractVec2(&coordinate, &centreVector), 1.0 / scale),
        &position
    );
}
Vec2 Camera::unMapCoordinate(Vec2 *coordinate)
{
    return addVec2(
        scaleVec2(subtractVec2(coordinate, &centreVector), 1.0 / scale),
        &position
    );
}

void Camera::tick(double delta)
{
    if (input->zoomInPressed())
        zoomIn(delta);
    if (input->zoomOutPressed())
        zoomOut(delta);
    
    if (input->upPressed())
        targetPosition.add(Vec2( 0.0, 1.0 * motionSpeed * delta * (1 / scale)));
    if (input->rightPressed())
        targetPosition.add(Vec2( 1.0 * motionSpeed * delta * (1 / scale), 0.0));
    if (input->downPressed())
        targetPosition.add(Vec2( 0.0,-1.0 * motionSpeed * delta * (1 / scale)));
    if (input->leftPressed())
        targetPosition.add(Vec2(-1.0 * motionSpeed * delta * (1 / scale), 0.0));

    previousPosition.set(&position);
    position.add(scaleVec2(subtractVec2(&targetPosition, &position), positionApproachQuotient * delta));
    previousScale = scale;
    scale += (targetScale - scale) * scaleApproachQuotient * delta;
}


void Camera::zoomIn(double delta)
{
    targetScale = clamp(targetScale * (1 + (zoomFactor - 1) * delta), minScale, maxScale);
}
void Camera::zoomOut(double delta)
{
    targetScale = clamp(targetScale / (1 + (zoomFactor - 1) * delta), minScale, maxScale);
}
