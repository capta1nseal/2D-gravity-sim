#include "particles.hpp"

#include <vector>
#include <cmath>

#include "vec2.hpp"
#include "attractor.hpp"

Particles::Particles() {}

void Particles::initializeParticles(double linearDensity, Vec2 topLeft, Vec2 bottomRight, Vec2 initialVelocity)
{
    if (linearDensity == 0)
    {
        m_particleCount = 0;
        m_positionArray.reserve(m_particleCount);
        m_previousPositionArray.reserve(m_particleCount);
        m_velocityArray.reserve(m_particleCount);
        m_attractorCount = 0;
        m_attractorArray.reserve(m_attractorCount);
        m_previousAttractorArray.reserve(m_attractorCount);
        m_attractorVelocityArray.reserve(m_attractorCount);

        return;
    }
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

    m_positionArray.resize(m_particleCount);
    m_previousPositionArray.resize(m_particleCount);
    m_velocityArray.resize(m_particleCount);

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
    Vec2 dPos;
    Vec2 force;
    Vec2 acceleration;

    for (Attractor currentAttractor : m_attractorArray)
    {
        for (unsigned int j = 0; j < m_particleCount; j++)
        {
            dPos = currentAttractor.position - m_positionArray[j];
            force = dPos * ((6.6743e-11 * currentAttractor.mass * 1.0) / pow(dPos.magnitude(), 3));
            acceleration = force / 1.0;
            m_velocityArray[j] += acceleration * delta;
        }
        for (unsigned int j = 0; j < m_attractorCount; j++)
        {
            if (currentAttractor.position == m_attractorArray[j].position) continue;

            dPos = currentAttractor.position - m_attractorArray[j].position;
            force = dPos * ((6.6743e-11 * currentAttractor.mass * m_attractorArray[j].mass) / pow(dPos.magnitude(), 3));
            acceleration = force / m_attractorArray[j].mass;
            m_attractorVelocityArray[j] += acceleration * delta;
        }
    }
    for (unsigned int i = 0; i < m_particleCount; i++)
    {
        m_positionArray[i] += m_velocityArray[i] * delta;
    }
    for (unsigned int i = 0; i < m_attractorCount; i++)
    {
        m_attractorArray[i].position += m_attractorVelocityArray[i] * delta;
    }
}

void Particles::addAttractor(Vec2 position, double mass)
{
    addAttractor(position, Vec2(), mass);
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

void Particles::removeAttractor(Vec2 position)
{
    if (m_attractorCount == 0) return;

    unsigned int closestIndex = 0;
    double closestDistance = INFINITY;
    double distance;

    for (unsigned int i = 0; i < m_attractorCount; i++)
    {
        distance = (m_attractorArray[i].position - position).magnitude();
        if (distance < closestDistance)
        {
            closestIndex = i;
            closestDistance = distance;
        }
    }

    removeAttractor(closestIndex);
}

Vec2* Particles::getClosestPositionPointer(Vec2 position)
{
    if (m_attractorCount == 0) return nullptr;

    unsigned int closestIndex = 0;
    double closestDistance = INFINITY;
    double distance;

    for (unsigned int i = 0; i < m_attractorCount; i++)
    {
        distance = (m_attractorArray[i].position - position).magnitude();
        if (distance < closestDistance)
        {
            closestIndex = i;
            closestDistance = distance;
        }
    }

    return &m_attractorArray[closestIndex].position;
}

void Particles::storePreviousPositions()
{
    m_previousPositionArray.resize(m_particleCount);
    m_previousAttractorArray.resize(m_attractorCount);

    m_previousPositionArray = m_positionArray;
    m_previousAttractorArray = m_attractorArray;
}

void Particles::removeAttractor(unsigned int index)
{
    m_attractorCount -= 1;

    m_attractorArray.erase(m_attractorArray.begin() + index);
    m_previousAttractorArray.erase(m_previousAttractorArray.begin() + index);
    m_attractorVelocityArray.erase(m_attractorVelocityArray.begin() + index);

    m_attractorArray.resize(m_attractorCount);
    m_previousAttractorArray.resize(m_attractorCount);
    m_attractorVelocityArray.resize(m_attractorCount);
}
