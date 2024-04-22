#ifndef _GRAVITYSIMPARTICLES_
#define _GRAVITYSIMPARTICLES_

#include <vector>
#include <cmath>

#include "vec2.hpp"
#include "particle.hpp"

class Particles
{
public:
    Particles();

    void generateParticles(double linearDensity, Vec2 topLeft, Vec2 bottomRight, Vec2 initialVelocity);

    void getFrameData(unsigned int &attractorCount, std::vector<Particle> &attractorArray, std::vector<Particle> &previousAttractorArray);

    void storePreviousPositions();

    void tick(double delta);

    void addAttractor(Vec2 position, double mass);
    void addAttractor(Vec2 position, Vec2 velocity, double mass);

    void removeAttractor(Vec2 position);

    Vec2* getClosestPositionPointer(Vec2 position);
private:
    unsigned int m_attractorCount;
    std::vector<Particle> m_particleArray;
    std::vector<Particle> m_previousParticleArray;

    void removeAttractor(unsigned int index);
};

#endif
