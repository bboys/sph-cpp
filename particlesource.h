#ifndef PARTICLESOURCE_H_ICS
#define PARTICLESOURCE_H_ICS

#include "graphics/base/scene.h"
#include "bucket.h"


class ParticleSource
{
    float d_mass_per_time;
    std::shared_ptr<Graphics::Base::Scene> *d_scene;
    std::shared_ptr<Physics::Buckets> d_buckets;
    float d_color[4];
    float d_position[3];
    float d_velocity[3];
    float d_rest;
    int spammed;
    int max;
public:
    ParticleSource(float position[3], float color[4], std::shared_ptr<Physics::Buckets> &buckets, std::shared_ptr<Graphics::Base::Scene> *scene, float mass_per_time, float velocity[3]);
    ParticleSource(float position[3], float color[4], std::shared_ptr<Physics::Buckets> &buckets, std::shared_ptr<Graphics::Base::Scene> *scene, float mass_per_time, float velocity[3], int max);
    void update(float tstep);
};

#endif
