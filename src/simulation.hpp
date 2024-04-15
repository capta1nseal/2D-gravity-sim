#ifndef _GRAVITYSIMSIMULATION_
#define _GRAVITYSIMSIMULATION_

#include <mutex>

#include "particles.hpp"

class GravitySimSimulation
{
public:
    GravitySimSimulation();

    void simulationLoop();

    void stop();

    void getFrameData(unsigned int &particleCount, std::vector<Vec2> &positionArray, std::vector<Vec2> &previousPositionArray, unsigned int &attractorCount, std::vector<Attractor> &attractorArray, std::vector<Attractor> &previousAttractorArray);

    void addAttractor(Vec2 position, double mass);
    void addAttractor(Vec2 position, Vec2 velocity, double mass);

    void removeAttractor(Vec2 position);

    Vec2* getClosestPositionPointer(Vec2 position);

    void storePreviousPositions();

private:
    bool running;

    // double tickRate;

    Particles particles;

    std::mutex simulationMutex;

    void initializeParticles();

    std::chrono::steady_clock::time_point now();

    void tick(double delta);

};

#endif
