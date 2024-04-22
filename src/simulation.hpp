#ifndef _GRAVITYSIMSIMULATION_
#define _GRAVITYSIMSIMULATION_

#include <mutex>

#include "particles.hpp"

class GravitySimSimulation
{
public:
    GravitySimSimulation();

    void generateParticles(double linearDensity, Vec2 topLeft, Vec2 bottomRight, Vec2 initialVelocity);

    void simulationLoop();

    void stop();

    void getFrameData(unsigned int &attractorCount, std::vector<Particle> &attractorArray, std::vector<Particle> &previousAttractorArray);

    void storePreviousPositions();

    void addAttractor(Vec2 position, double mass);
    void addAttractor(Vec2 position, Vec2 velocity, double mass);

    void removeAttractor(Vec2 position);

    Vec2* getClosestPositionPointer(Vec2 position);

    void setTimeScale(double newTimeScale);
    double getTimeScale();

    void increaseTimeScale(double factor);
    void decreaseTimeScale(double factor);

private:
    bool running;

    double timeScale;

    // double tickRate;

    Particles particles;

    std::mutex simulationMutex;

    std::chrono::steady_clock::time_point now();

    void tick(double delta);

};

#endif
