#ifndef PARTICLESOURCE_H_ICS
#define PARTICLESOURCE_H_ICS

#include "particle.h"
#include "bucket.h"


class ParticleSource
{
    float d_mass_per_time;
    ParticlesBase *d_particles;
    Buckets *d_buckets;
    float d_color[4];
    float d_position[3];
    float d_velocity[3];
    float d_rest;
    int spammed;
    int max;
public:
    ParticleSource(float position[3], float color[4], Buckets *buckets, ParticlesBase *particles, float mass_per_time, float velocity[3]);
    ParticleSource(float position[3], float color[4], Buckets *buckets, ParticlesBase *particles, float mass_per_time, float velocity[3], int max);
    void update(float tstep);
};

#endif
