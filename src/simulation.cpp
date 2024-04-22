#include "simulation.hpp"

#include <mutex>
#include <chrono>

#include "particles.hpp"

GravitySimSimulation::GravitySimSimulation()
{
    timeScale = 1.0;
}

void GravitySimSimulation::generateParticles(double linearDensity, Vec2 topLeft, Vec2 bottomRight, Vec2 initialVelocity)
{
    particles.generateParticles(linearDensity, topLeft, bottomRight, initialVelocity);
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

void GravitySimSimulation::getFrameData(unsigned int &attractorCount, std::vector<Particle> &attractorArray, std::vector<Particle> &previousAttractorArray)
{
    std::lock_guard<std::mutex> lock(simulationMutex);
    particles.getFrameData(attractorCount, attractorArray, previousAttractorArray);
    particles.storePreviousPositions();
}

void GravitySimSimulation::storePreviousPositions()
{
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

void GravitySimSimulation::setTimeScale(double newTimeScale)
{
    timeScale = newTimeScale;
}
double GravitySimSimulation::getTimeScale()
{
    return timeScale;
}

void GravitySimSimulation::increaseTimeScale(double factor)
{
    timeScale *= factor;
}
void GravitySimSimulation::decreaseTimeScale(double factor)
{
    timeScale /= factor;
}

std::chrono::steady_clock::time_point GravitySimSimulation::now()
{
    return std::chrono::steady_clock::now();
}

void GravitySimSimulation::tick(double delta)
{
    particles.tick(delta * timeScale);
}
