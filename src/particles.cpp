#include "particles.hpp"

#include <iostream>

#include <vector>
#include <cmath>

#include "vec2.hpp"
#include "camera.hpp"

Particles::Particles() {}

void Particles::generateParticles(double linearDensity, Vec2 topLeft, Vec2 bottomRight, Vec2 initialVelocity)
{
    m_particleCount = 0;
    for (double x = topLeft.x; x < bottomRight.x; x += 1.0 / linearDensity)
    {
        for (double y = topLeft.y; y < bottomRight.y; y += 1.0 / linearDensity)
        {
            m_particleCount += 1;
        }
    }

    m_positionArray.reserve(m_particleCount);
    m_previousPositionArray.reserve(m_particleCount);
    m_velocityArray.reserve(m_particleCount);

    for (double x = topLeft.x; x < bottomRight.x; x += 1.0 / linearDensity)
    {
        for (double y = topLeft.y; y < bottomRight.y; y += 1.0 / linearDensity)
        {
            m_positionArray.push_back(Vec2(x, y));
            m_previousPositionArray.push_back(Vec2(x, y));
            m_velocityArray.push_back(initialVelocity);
        }
    }

    m_attractorCount = 0;
    m_attractorArray.reserve(m_attractorCount);
    m_previousAttractorArray.reserve(m_attractorCount);
    m_attractorVelocityArray.reserve(m_attractorCount);
}

void Particles::getFrameData(unsigned int &particleCount, std::vector<Vec2> &positionArray, std::vector<Vec2> &previousPositionArray, unsigned int &attractorCount, std::vector<Attractor> &attractorArray, std::vector<Attractor> &previousAttractorArray)
{
    particleCount = m_particleCount;
    
    positionArray.resize(particleCount);
    previousPositionArray.resize(particleCount);

    positionArray = m_positionArray;
    previousPositionArray = m_previousPositionArray;

    attractorCount = m_attractorCount;

    attractorArray.resize(attractorCount);
    previousAttractorArray.resize(attractorCount);

    attractorArray = m_attractorArray;
    previousAttractorArray = m_previousAttractorArray;
}

void Particles::tick(double delta)
{
    double dx;
    double dy;
    double forceVectorOverDistance;

    for (Attractor currentAttractor : m_attractorArray)
    {
        for (int j = 0; j < m_particleCount; j++)
        {
            dx = currentAttractor.position.x - m_positionArray[j].x;
            dy = currentAttractor.position.y - m_positionArray[j].y;
            forceVectorOverDistance = (10.0 * currentAttractor.mass) / pow(Vec2(dx, dy).magnitude(), 3);
            m_velocityArray[j].add(Vec2(forceVectorOverDistance * dx, forceVectorOverDistance * dy));
        }
        for (int j = 0; j < m_attractorCount; j++)
        {
            if ((currentAttractor.position.x == m_attractorArray[j].position.x) and (currentAttractor.position.y == m_attractorArray[j].position.y)) continue;
            dx = currentAttractor.position.x - m_attractorArray[j].position.x;
            dy = currentAttractor.position.y - m_attractorArray[j].position.y;
            forceVectorOverDistance = (10.0 * currentAttractor.mass * m_attractorArray[j].mass) / pow(Vec2(dx, dy).magnitude(), 3);
            m_attractorVelocityArray[j].add(Vec2(forceVectorOverDistance * dx, forceVectorOverDistance * dy));
        }
    }
    for (int i = 0; i < m_particleCount; i++)
    {
        m_positionArray[i].add(m_velocityArray[i].scaleInplace(delta));
    }
    for (int i = 0; i < m_attractorCount; i++)
    {
        m_attractorArray[i].position.add(m_attractorVelocityArray[i].scaleInplace(delta));
    }
}

void Particles::addAttractor(Vec2 position, double mass)
{
    addAttractor(position, Vec2(0.0, 0.0), mass);
}
void Particles::addAttractor(Vec2 position, Vec2 velocity, double mass)
{
    m_attractorCount++;
    m_attractorArray.reserve(m_attractorCount);
    m_previousAttractorArray.reserve(m_attractorCount);
    m_attractorVelocityArray.reserve(m_attractorCount);

    m_attractorArray.push_back(Attractor(position, mass));
    m_previousAttractorArray.push_back(Attractor(position, mass));
    m_attractorVelocityArray.push_back(velocity);
}

void Particles::storePreviousPositions()
{
    m_previousPositionArray.resize(m_particleCount);
    m_previousAttractorArray.resize(m_attractorCount);
    m_previousPositionArray = m_positionArray;
    m_previousAttractorArray = m_attractorArray;
}
