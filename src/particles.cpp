#include "particles.hpp"

#include <vector>
#include <cmath>

#include <iostream>

#include "vec2.hpp"
#include "particle.hpp"

Particles::Particles()
{
    m_attractorCount = 0;
    m_particleArray.reserve(m_attractorCount);
    m_previousParticleArray.reserve(m_attractorCount);
}

void Particles::generateParticles(double linearDensity, Vec2 topLeft, Vec2 bottomRight, Vec2 initialVelocity)
{
    for (double x = topLeft.x; x < bottomRight.x; x += 1.0 / linearDensity)
    {
        for (double y = topLeft.y; y < bottomRight.y; y += 1.0 / linearDensity)
        {
            m_attractorCount += 1;
        }
    }

    m_particleArray.reserve(m_attractorCount);
    m_previousParticleArray.reserve(m_attractorCount);

    for (double x = topLeft.x; x < bottomRight.x; x += 1.0 / linearDensity)
    {
        for (double y = topLeft.y; y < bottomRight.y; y += 1.0 / linearDensity)
        {
            m_particleArray.push_back(Particle(Vec2(x, y), initialVelocity, 0.0));
            m_previousParticleArray.push_back(Particle(Vec2(x, y), initialVelocity, 0.0));
        }
    }
}

void Particles::getFrameData(unsigned int &attractorCount, std::vector<Particle> &attractorArray, std::vector<Particle> &previousAttractorArray)
{
    attractorCount = m_attractorCount;

    attractorArray.resize(attractorCount);
    previousAttractorArray.resize(attractorCount);

    attractorArray = m_particleArray;
    previousAttractorArray = m_previousParticleArray;
}

void Particles::storePreviousPositions()
{
    m_previousParticleArray.resize(m_attractorCount);
    m_previousParticleArray = m_particleArray;
}

void Particles::tick(double delta)
{
    double mass;
    Vec2 dPos;
    Vec2 force;
    Vec2 acceleration;

    for (Particle currentAttractor : m_particleArray)
    {
        if (currentAttractor.mass == 0.0) continue;

        for (unsigned int j = 0; j < m_attractorCount; j++)
        {
            if (m_particleArray[j].mass == 0.0) mass = 1.0;
            else mass = m_particleArray[j].mass;

            if (currentAttractor.position == m_particleArray[j].position) continue;

            dPos = currentAttractor.position - m_particleArray[j].position;
            force = dPos * ((6.6743e-11 * currentAttractor.mass * mass) / pow(dPos.magnitude(), 3));
            acceleration = force / mass;
            m_particleArray[j].velocity += acceleration * delta;
        }
    }
    for (unsigned int i = 0; i < m_attractorCount; i++)
    {
        m_particleArray[i].position += m_particleArray[i].velocity * delta;
    }
}

void Particles::addAttractor(Vec2 position, double mass)
{
    addAttractor(position, Vec2(), mass);
}
void Particles::addAttractor(Vec2 position, Vec2 velocity, double mass)
{
    m_attractorCount++;
    m_particleArray.reserve(m_attractorCount);
    m_previousParticleArray.reserve(m_attractorCount);

    m_particleArray.push_back(Particle(position, velocity, mass));
    m_previousParticleArray.push_back(Particle(position, velocity, mass));
}

void Particles::removeAttractor(Vec2 position)
{
    if (m_attractorCount == 0) return;

    unsigned int closestIndex = 0;
    double closestDistance = INFINITY;
    double distance;

    for (unsigned int i = 0; i < m_attractorCount; i++)
    {
        distance = (m_particleArray[i].position - position).magnitude();
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
        distance = (m_particleArray[i].position - position).magnitude();
        if (distance < closestDistance)
        {
            closestIndex = i;
            closestDistance = distance;
        }
    }

    return &m_particleArray[closestIndex].position;
}

void Particles::removeAttractor(unsigned int index)
{
    m_attractorCount -= 1;

    m_particleArray.erase(m_particleArray.begin() + index);
    m_previousParticleArray.erase(m_previousParticleArray.begin() + index);

    m_particleArray.resize(m_attractorCount);
    m_previousParticleArray.resize(m_attractorCount);
}
