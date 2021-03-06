#ifndef SPH_PHYSICS_PARTICLE_H
#define SPH_PHYSICS_PARTICLE_H

#include "utils.h"
#include <vector>

namespace Physics {

class Bucket;
class Particle;
class Buckets;

extern float Acc_ext[3];
extern float gravity[3];
extern float gravity_0[3];

extern float stiffness;
extern float sigma;
extern float mu;
extern float r_0;
extern float radius_factor;
extern float mass_0;
extern float density_0;
extern float pressure_0;

extern float num_neighbour_particles;

class Particle
{
public:
    float position[3];
    float radius;
    float velocity[3];
    float normal[3];
    float acceleration[3];
    float mass;
    float density;
    float pressure;

    float effective_radius;
    float effective_radius2;

    float dt;
    float DDcs;

    float F_press[3];
    float F_vis[3];
    float F_surface[3];

protected:
    Bucket *d_bucket;
    Buckets *d_buckets;
    static std::vector<float> wall_dens_vec;
    static std::vector<float> wall_vis_vec;

public:
    Particle() = delete;
    ~Particle();
    Particle(float position[3], Buckets *buckets);
    Particle(const Particle &other);
    Particle(Particle &&other);

    Particle &operator=(Particle const &other);
    Particle &operator=(Particle &&other);

    static void init_wall();

    float update_density();
    void update(float tstep);
    void update_density(float tstep);
    void update_position(float tstep);

protected:
    void swap(Particle &other);

    void update_press();
    void update_forces();

    void update_F_press(Particle *particle_i, Difference const &diff);
    void update_F_vis(Particle *particle_i, Difference const &diff);
    void update_F_surface(Particle *particle_i, Difference const &diff);
    void update_normal(Particle *particle_i, Difference const &diff);

    float W(float plen2) const;
    float W(Difference const &diff) const;
    void DW(Difference const &diff, float out[3]) const;
    float DDW(Difference const &diff) const;
    void DW_press(Difference const &diff, float out[3]) const;
    float DDW_vis(Difference const &diff) const;

    void update_bucket(bool force = false);

    float Z_wall_dens(const float distance);
    float Z_wall_vis(const float distance);

    float wall_distance();
    void wall_vector(float out[3]);
    void get_normal_by_idx(float out[3], size_t idx);
};

class ParticlesBase : public std::vector<Particle>
{
public:
    ParticlesBase();
    virtual ~ParticlesBase();
    void update(float tstep);
};

}

#endif
