#include "simulation.hpp"

#include <mutex>
#include <chrono>

#include "particles.hpp"

GravitySimSimulation::GravitySimSimulation()
{
    initializeParticles();
}

void GravitySimSimulation::simulationLoop()
{
    auto start = now();
    std::chrono::duration<double> delta(0.0);

    int tickCounter = 0;

    running = true;
    while (running)
    {
        start = now();

        tick(delta.count());

        delta = now() - start;

        tickCounter++;
    }
}

void GravitySimSimulation::stop()
{
    running = false;
}

void GravitySimSimulation::getFrameData(unsigned int &particleCount, std::vector<Vec2> &positionArray, std::vector<Vec2> &previousPositionArray, unsigned int &attractorCount, std::vector<Attractor> &attractorArray, std::vector<Attractor> &previousAttractorArray)
{
    std::lock_guard<std::mutex> lock(simulationMutex);
    particles.getFrameData(particleCount, positionArray, previousPositionArray, attractorCount, attractorArray, previousAttractorArray);
    particles.storePreviousPositions();
}

void GravitySimSimulation::addAttractor(Vec2 position, double mass)
{
    particles.addAttractor(position, mass);
}
void GravitySimSimulation::addAttractor(Vec2 position, Vec2 velocity, double mass)
{
    particles.addAttractor(position, velocity, mass);
}

void GravitySimSimulation::removeAttractor(Vec2 position)
{
    particles.removeAttractor(position);
}

Vec2* GravitySimSimulation::getClosestPositionPointer(Vec2 position)
{
    return particles.getClosestPositionPointer(position);
}

void GravitySimSimulation::storePreviousPositions()
{
    particles.storePreviousPositions();
}

void GravitySimSimulation::initializeParticles()
{
    double astronomicalUnit = 149597870700.0;

    particles.initializeParticles(5e-9, Vec2(2.6 * astronomicalUnit, -0.1 * astronomicalUnit), Vec2(2.8 * astronomicalUnit, 0.1 * astronomicalUnit), Vec2(0.0, 18500.0));
}

std::chrono::steady_clock::time_point GravitySimSimulation::now()
{
    return std::chrono::steady_clock::now();
}

void GravitySimSimulation::tick(double delta)
{
    particles.tick(delta * 1e7);
}
