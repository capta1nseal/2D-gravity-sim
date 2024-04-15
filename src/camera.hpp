#ifndef _GRAVITYSIMCAMERA_
#define _GRAVITYSIMCAMERA_

#include "vec2.hpp"
#include "input.hpp"

class Camera
{
public:
    Camera();

    void initializeResolution(int initialDisplayWidth, int initialDisplayHeight);

    void setInput(Input *inputPtr);

    void setPosition(Vec2 newPosition);

    void setTargetPosition(Vec2 newTargetPosition);
    void setTargetPosition(Vec2 *newTargetPosition);

    void setScale(double newScale);

    void setTargetScale(double newTargetScale);

    double getScale();

    Vec2 mapCoordinate(Vec2 coordinate);
    Vec2 mapCoordinate(Vec2 *coordinate);

    Vec2 mapPreviousCoordinate(Vec2 coordinate);
    Vec2 mapPreviousCoordinate(Vec2 *coordinate);

    Vec2 unMapCoordinate(Vec2 coordinate);
    Vec2 unMapCoordinate(Vec2 *coordinate);

    bool isFollowing();

    void startFollowing(Vec2* positionToFollow);
    void stopFollowing();

    void tick(double delta);

private:
    Vec2 centreVector;
    Vec2 position;
    Vec2 previousPosition;
    Vec2 targetPosition;
    double positionApproachQuotient;
    double motionSpeed;

    bool following;
    Vec2* followingPosition;

    double minScale;
    double maxScale;
    double scale;
    double previousScale;
    double targetScale;
    double scaleApproachQuotient;

    double zoomFactor;

    Input *input;

    void zoomIn(double delta);
    void zoomOut(double delta);
};

#endif
