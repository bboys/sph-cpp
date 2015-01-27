#include "particlesource.h"
#include <cstring>
#include <cmath>
#include "utils.h"

ParticleSource::ParticleSource(float position[3], float color[4], Physics::Buckets *buckets, std::shared_ptr<Graphics::Base::Particles> *particles, float mass_per_time, float velocity[3])
:
    d_mass_per_time(mass_per_time),
    d_particles(particles),
    d_buckets(buckets),
    d_rest(0.0f),
    spammed(0),
    max(1500)
{
    memcpy(d_color, color, sizeof(float)*4);
    memcpy(d_position, position, sizeof(float)*3);
    memcpy(d_velocity, velocity, sizeof(float)*3);
}

ParticleSource::ParticleSource(float position[3], float color[4], Physics::Buckets *buckets, std::shared_ptr<Graphics::Base::Particles> *particles, float mass_per_time, float velocity[3], int max)
:
    d_mass_per_time(mass_per_time),
    d_particles(particles),
    d_buckets(buckets),
    d_rest(0.0f),
    spammed(0),
    max(max)
{
    memcpy(d_color, color, sizeof(float)*4);
    memcpy(d_position, position, sizeof(float)*3);
    memcpy(d_velocity, velocity, sizeof(float)*3);
}

void ParticleSource::update(float t_step)
{
    if (spammed >= max) return;
    float avg = d_mass_per_time / Physics::mass_0 * t_step;
    float pos[3];
    memcpy(pos, d_position, sizeof(float)*3);
    for (int  i = 0; i != static_cast<int>(avg+d_rest); ++i)
    {
        pos[0] += frand(-1e-6,1e-6);
        pos[1] += frand(-1e-6,1e-6);
        pos[2] += frand(-1e-6,1e-6);
        Graphics::Base::Particle particle(pos, d_color);
        (*d_particles)->push_back(particle);
        Physics::Particle phys_particle(pos, d_buckets);
        (*d_particles)->get_particles()->push_back(phys_particle);
        memcpy((*d_particles)->get_particles()->back().velocity, d_velocity, sizeof(float)*3);
        if (pos[1] + 2*Physics::r_0 < 1) pos[1] += 2*Physics::r_0;
        else if (pos[0] + 2*Physics::r_0 < 1) pos[0] += 2*Physics::r_0;
        else pos[2] -= 2*Physics::r_0;
        ++spammed;
    }
    d_rest = avg+d_rest - floor(avg+d_rest);
}
