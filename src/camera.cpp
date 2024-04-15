#include "camera.hpp"

#include "vec2.hpp"
#include "input.hpp"
#include "math.hpp"


Camera::Camera()
{
    positionApproachQuotient = 5.0;
    motionSpeed = 1.0;

    following = false;
    followingPosition = nullptr;

    minScale = 1e-15;
    maxScale = 1e15;
    scale = 2.5e-13;
    previousScale = scale;
    targetScale = scale;
    scaleApproachQuotient = 10.0;

    zoomFactor = 10.0;
}

void Camera::initializeResolution(int initialDisplayWidth, int initialDisplayHeight)
{
    centreVector = Vec2(initialDisplayWidth / 2.0, initialDisplayHeight / 2.0);
}

void Camera::setInput(Input *inputPtr)
{
    input = inputPtr;
}

void Camera::setPosition(Vec2 newPosition)
{
    targetPosition = newPosition;
    previousPosition = newPosition;
    position = newPosition;
}

void Camera::setTargetPosition(Vec2 newTargetPosition)
{
    targetPosition = newTargetPosition;
}

void Camera::setTargetPosition(Vec2 *newTargetPosition)
{
    targetPosition = *newTargetPosition;
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
    Vec2 mappedCoordinate = (*coordinate - position) * scale;
    mappedCoordinate.y *= centreVector.x / centreVector.y;
    return mappedCoordinate;
}

Vec2 Camera::mapPreviousCoordinate(Vec2 coordinate)
{
    return mapPreviousCoordinate(&coordinate);
}
Vec2 Camera::mapPreviousCoordinate(Vec2 *coordinate)
{
    Vec2 mappedCoordinate = (*coordinate - position) * previousScale;
    mappedCoordinate.y *= centreVector.x / centreVector.y;
    return mappedCoordinate;
}

Vec2 Camera::unMapCoordinate(Vec2 coordinate)
{
    return unMapCoordinate(&coordinate);
}
Vec2 Camera::unMapCoordinate(Vec2 *coordinate)
{
    Vec2 mappedCoordinate = Vec2(
        (coordinate->x - centreVector.x) / centreVector.x,
        (centreVector.y - coordinate->y) / centreVector.x
    );

    return mappedCoordinate / scale + position;
}

bool Camera::isFollowing()
{
    return following;
}

void Camera::startFollowing(Vec2* positionToFollow)
{
    if (positionToFollow == nullptr) return;

    followingPosition = positionToFollow;
    following = true;
}

void Camera::stopFollowing()
{
    following = false;
    followingPosition = nullptr;
}

void Camera::tick(double delta)
{
    if (input->zoomInPressed())
        zoomIn(delta);
    if (input->zoomOutPressed())
        zoomOut(delta);
    
    if (input->upPressed())
        targetPosition += Vec2( 0.0, 1.0 * motionSpeed * delta * (1.0 / scale));
    if (input->rightPressed())
        targetPosition += Vec2( 1.0 * motionSpeed * delta * (1.0 / scale), 0.0);
    if (input->downPressed())
        targetPosition += Vec2( 0.0,-1.0 * motionSpeed * delta * (1.0 / scale));
    if (input->leftPressed())
        targetPosition += Vec2(-1.0 * motionSpeed * delta * (1.0 / scale), 0.0);

    previousPosition = position;
    if (following)
    {
        targetPosition = *followingPosition;
        position = targetPosition;
    }
    else
    {
        position += (targetPosition - position) * (positionApproachQuotient * delta);
    }
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
