#ifndef _GRAVITYSIMPARTICLES_
#define _GRAVITYSIMPARTICLES_

#include <vector>
#include <cmath>

#include "vec2.hpp"
#include "attractor.hpp"

class Particles
{
public:
    Particles();

    void initializeParticles(double linearDensity, Vec2 topLeft, Vec2 bottomRight, Vec2 initialVelocity);

    void getFrameData(unsigned int &particleCount, std::vector<Vec2> &positionArray, std::vector<Vec2> &previousPositionArray, unsigned int &attractorCount, std::vector<Attractor> &attractorArray, std::vector<Attractor> &previousAttractorArray);

    void tick(double delta);

    void addAttractor(Vec2 position, double mass);
    void addAttractor(Vec2 position, Vec2 velocity, double mass);

    void removeAttractor(Vec2 position);

    Vec2* getClosestPositionPointer(Vec2 position);

    void storePreviousPositions();

private:
    int m_particleCount;
    std::vector<Vec2> m_positionArray;
    std::vector<Vec2> m_previousPositionArray;
    std::vector<Vec2> m_velocityArray;

    int m_attractorCount;
    std::vector<Attractor> m_attractorArray;
    std::vector<Attractor> m_previousAttractorArray;
    std::vector<Vec2> m_attractorVelocityArray;

    void removeAttractor(unsigned int index);
};

#endif
