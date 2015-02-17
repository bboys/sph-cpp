#include "particle.h"
#include <iostream>
#include "bucket.h"
#include <cmath>
#include <cassert>
#include <cstring>

namespace Physics {

//~ const float mass_0 = 1000000.0;
//~ const float mass_0 = 100.0;
//~ const float mass_0 = 1.0;
float gravity_0[3] = {0.0, -9.81, 0.0};
float gravity[3];
//~ const float gravity = 0.0;
float Acc_ext[3] = {0, 0, 0};
float mass_0 = 0.012;
//~ const float mass_0 = 1.0;
//~ const float mass_0 = 0.0001;
float density_0 = 1000.0; // kg/m^3
//~ const float density_0 = 1000.0;
//~ const float pressure_0 = 101325.0;
float pressure_0 = 0.0;
//~ const float sigma = 10.0; // tension coefficient
//~ const float sigma = 1e1; // tension coefficient
//~ const float sigma = 1.0; // tension coefficient
//~ const float sigma = 7.28e-2; // tension coefficient
//~ float sigma = 0.0001; // tension coefficient
float sigma = 0.0004; // realistic tension coefficient
//~ const float stiffness = 0.0;
//~ const float stiffness = 0.04;
float stiffness = 0.4;
//~ const float stiffness = 1.0;
//~ const float stiffness = 5e-1;
//~ const float stiffness = 0.1;
//~ const float stiffness = 1497.0f * 1497.0f; // Speed of sound squared
float mu = 0.5f;
//~ const float mu = 0.0f;
//~ const float mu = 1e-3f;
//~ const float mu = 0.5f;t
//~ const float mu = 5e-1;
float r_0 = 0.05;
float radius_factor = 2.0f;
float eps = 1e-15;

std::vector<float> Particle::wall_dens_vec = std::vector<float>();
std::vector<float> Particle::wall_vis_vec = std::vector<float>();
float num_neighbour_particles = 0.0f;


Particle::Particle(float p[3], Buckets *buckets)
:
    radius(r_0),
    velocity{0.0f, 0.0f, 0.0f},
    normal{},
    acceleration{},
    mass(mass_0),
    density(density_0),
    pressure(pressure_0),
    effective_radius(radius * radius_factor),
    effective_radius2(effective_radius * effective_radius),
    dt(0.0f),
    DDcs(0.0f),
    F_press{},
    F_vis{},
    F_surface{},
    d_bucket(NULL),
    d_buckets(buckets)
{
    memcpy(position, p, sizeof(float)*3);
    update_bucket(true);
}

Particle::~Particle()
{
    if (d_bucket) d_bucket->remove_particle(this);
}

Particle::Particle(Particle const &other)
:
    radius(other.radius),
    mass(other.mass),
    density(other.density),
    pressure(other.pressure),
    effective_radius(other.effective_radius),
    effective_radius2(other.effective_radius2),
    dt(other.dt),
    DDcs(other.DDcs),
    d_bucket(NULL),
    d_buckets(other.d_buckets)
{
    static const size_t sizeofvec = sizeof(float)*3;
    memcpy(position, other.position, sizeofvec);
    memcpy(velocity, other.velocity, sizeofvec);
    memcpy(normal, other.normal, sizeofvec);
    memcpy(acceleration, other.acceleration, sizeofvec);
    memcpy(F_press, other.F_press, sizeofvec);
    memcpy(F_vis, other.F_vis, sizeofvec);
    memcpy(F_surface, other.F_surface, sizeofvec);
    update_bucket(true);
}

Particle::Particle(Particle &&other)
:
    radius(other.radius),
    mass(other.mass),
    density(other.density),
    pressure(other.pressure),
    effective_radius(other.effective_radius),
    effective_radius2(other.effective_radius2),
    dt(other.dt),
    DDcs(other.DDcs),
    d_bucket(NULL),
    d_buckets(other.d_buckets)
{
    static const size_t sizeofvec = sizeof(float)*3;
    memcpy(position, other.position, sizeofvec);
    memcpy(velocity, other.velocity, sizeofvec);
    memcpy(normal, other.normal, sizeofvec);
    memcpy(acceleration, other.acceleration, sizeofvec);
    memcpy(F_press, other.F_press, sizeofvec);
    memcpy(F_vis, other.F_vis, sizeofvec);
    memcpy(F_surface, other.F_surface, sizeofvec);
    update_bucket(true);
}

Particle &Particle::operator=(Particle const &other)
{
    Particle tmp(other);
    swap(tmp);
    update_bucket(true);
    return *this;
}

Particle &Particle::operator=(Particle &&other)
{
    swap(other);
    update_bucket(true);
    return *this;
}

void Particle::swap(Particle &other)
{
    char buffer[sizeof(Particle)];
    memcpy(buffer, &other, sizeof(Particle));
    memcpy(&other, this,   sizeof(Particle));
    memcpy(this,   buffer, sizeof(Particle));
}

void Particle::get_normal_by_idx(float out[3], size_t idx)
{
    if (position[idx] < 0.0f)
        out[idx] = 1.0f;
    else
        out[idx] = -1.0f;
}

void Particle::wall_vector(float out[3])
{
    set_zero(out);

    float r1 = std::min(1.0f-position[0], 1.0f+position[0]);
    float r2 = std::min(1.0f-position[1], 1.0f+position[1]);
    float r3 = std::min(1.0f-position[2], 1.0f+position[2]);
    if (r1 < r2)
    {
        if (r1 < r3)
            return get_normal_by_idx(out, 0);
        else
            return get_normal_by_idx(out, 2);
    }
    else if (r2 < r3)
        return get_normal_by_idx(out, 1);
    else
        return get_normal_by_idx(out, 2);
}

float Particle::wall_distance()
{
    return std::min(std::min(std::min(std::min(std::min(1.0f-position[0], 1.0f+position[0]), 1.0f+position[1]), 1.0f-position[1]), 1.0f+position[2]), 1.0f-position[2]);
}

float Particle::update_density()
{
    density = 0.0f;
    for (Bucket::neighbour_iterator it = d_bucket->begin(); it; ++it)
    {
        Particle *particle_i = *it;
        Difference diff;
        difference(position, particle_i->position, diff);

        if (diff.len > effective_radius)
            continue;

        density += particle_i->mass * W(diff);
    }
    return density;
}

void Particle::update_F_press(Particle *particle_i, Difference const &diff)
{
    float c = particle_i->mass * (pressure + particle_i->pressure) / 2.0f / particle_i->density;
    float v[3] = {0.0f, 0.0f, 0.0f};
    DW_press(diff, v);
    F_press[0] -= c*v[0];
    F_press[1] -= c*v[1];
    F_press[2] -= c*v[2];
}

void Particle::update_F_vis(Particle *particle_i, Difference const &diff)
{
    float c = particle_i->mass / particle_i->density * DDW_vis(diff);
    F_vis[0] += c*(particle_i->velocity[0] - velocity[0]);
    F_vis[1] += c*(particle_i->velocity[1] - velocity[1]);
    F_vis[2] += c*(particle_i->velocity[2] - velocity[2]);
}

void Particle::update_F_surface(Particle *particle_i, Difference const &diff)
{
    float c = particle_i->mass * W(diff); // surface model 1
    F_surface[0] -= c * diff.vec[0]; // surface model 1
    F_surface[1] -= c * diff.vec[1]; // surface model 1
    F_surface[2] -= c * diff.vec[2]; // surface model 1

    //~ float v[3];
    //~ DW(diff, v);
    //~ F_surface[0] -= particle_i->mass * v[0]; // surface model 3
    //~ F_surface[1] -= particle_i->mass * v[1]; // surface model 3
    //~ F_surface[2] -= particle_i->mass * v[2]; // surface model 3
}

void Particle::update_normal(Particle *particle_i, Difference const &diff)
{
    float c = particle_i->mass / particle_i->density;
    //~ DDcs += c * DDW(diff); // surface model 2
    float v[3];
    DW(diff, v);
    normal[0] -= v[0] * c;
    normal[1] -= v[1] * c;
    normal[2] -= v[2] * c;
    //~ DDcs += c * (particle_i->normal[0] * v[0] + particle_i->normal[1] * v[1] + particle_i->normal[2] * v[2]); // surface model 5
}

void Particle::update_forces()
{
    DDcs = 0.0f;
    set_zero(F_press);
    set_zero(F_vis);
    set_zero(F_surface);
    set_zero(normal);

    size_t i = 0;
    for (Bucket::neighbour_iterator it = d_bucket->begin(); it; ++it)
    {
        Particle *particle_i = *it;
        if (particle_i == this)
            continue;

        Difference diff;
        difference(position, particle_i->position, diff);

        if (diff.len > effective_radius)
            continue;

        update_normal(particle_i, diff);
        update_F_press(particle_i, diff);
        update_F_vis(particle_i, diff);
        update_F_surface(particle_i, diff);
        ++i;
    }
    num_neighbour_particles = (num_neighbour_particles * 5000 - num_neighbour_particles + i) / 5000;

    for (size_t idx = 0; idx != 3; ++idx)
    {
        F_vis[idx] *= mu;
        F_surface[idx] *= sigma * density / mass;
    }

    // Wall pressure update
    float d = wall_distance();
    if (d < radius)
    {
        float wall_normal[3];
        wall_vector(wall_normal);
        //~ memcpy(color, wall_normal, sizeof(float)*3);
        for (size_t idx = 0; idx != 3; ++idx)
            F_press[idx] += density * (radius - d) * wall_normal[idx] / dt / dt;
    }

    // Normal update
    float nlen = length(normal);
    if (nlen < 1.0e-2f)
    {
        set_zero(normal);
        //~ set_zero(F_surface); // surface model 2, 4, 5
    }
    //~ else // surface model 2, 4, 5
        //~ for (size_t idx = 0; idx != 3; ++idx) // surface model 2, 4, 5
            //~ F_surface[idx] = sigma * DDcs * normal[idx] / nlen; // surface model 2, 5
            //~ F_surface[idx] = sigma * density * density / mass * normal[idx]; // surface model 4

    //~ float l = length(F_press) / 1000.0;
    //~ float l2[3] = {l,l,l};
    //~ memcpy(color, l2, sizeof(float)*3);
    //~ memcpy(color, F_press, sizeof(float)*3);
}
    
float Particle::W(float plen2) const
{
    static const float coef = 315.0f / 64.0f / M_PI / pow(effective_radius, 9.0f);
    return coef * pow(effective_radius2 - plen2, 3.0f);
}

float Particle::W(Difference const &diff) const
{
    return W(diff.len2);
}

void Particle::DW(Difference const &diff, float out[3]) const
{
    //~ static const float er2 = effective_radius * 2.0f;
    //~ if (diff.len > er2)
        //~ return;
//~ 
    //~ static const float coef = 945.0f / 32.0f / M_PI / pow(er2, 9.0f);
    //~ float c = coef * pow(er2 * er2 - diff.len2, 2.0f);
    //~ for (size_t idx = 0; idx != 3; ++idx)
        //~ out[idx] = c * diff.vec[idx];

    static const float coef = 945.0f / 32.0f / M_PI / pow(effective_radius, 9.0f);
    float c = coef * pow(effective_radius2 - diff.len2, 2.0f);
    out[0] = c * diff.vec[0];
    out[1] = c * diff.vec[1];
    out[2] = c * diff.vec[2];
}

float Particle::DDW(Difference const &diff) const
{
    static const float coef = -945.0f / 32.0f / M_PI / pow(effective_radius, 9.0f);
    return coef * (3.0f * effective_radius2 - 7.0f * diff.len2) * (effective_radius2 - diff.len2);
}

void Particle::DW_press(Difference const &diff, float out[3]) const
{
    if (diff.len < eps)
        return;

    static const float coef = 45.0f / M_PI / pow(effective_radius, 6.0f);
    float c = coef * pow(effective_radius - diff.len, 2.0f) / diff.len;
    out[0] = c * diff.vec[0];
    out[1] = c * diff.vec[1];
    out[2] = c * diff.vec[2];
}

float Particle::DDW_vis(Difference const &diff) const
{
    static const float coef = 45.0 / M_PI / pow(effective_radius, 6.0f);
    return coef * (effective_radius - diff.len);
}

void Particle::update_bucket(bool force)
{
    Bucket *newbucket = d_buckets->get_bucket(position);
    if (newbucket == d_bucket and !force) return;

    if (d_bucket) d_bucket->remove_particle(this);
    d_bucket = newbucket;
    d_bucket->add_particle(this);
}

void Particle::update_density(float tstep)
{
    dt = tstep;
    update_density();
    density += Z_wall_dens(wall_distance());
}

void Particle::update(float tstep)
{
    float d = wall_distance();
    dt = tstep;
    update_press();
    update_forces();
    //~ dump(pressure);
    //~ dump(density);
    assert(!isnan(density));
    set_zero(acceleration);
    for (int i = 0; i != 3; ++i)
    {
        assert(!isnan(F_press[i]));
        assert(!isnan(F_vis[i]));
        assert(!isnan(F_surface[i]));
        F_vis[i] += velocity[i] * Z_wall_vis(d);
        acceleration[i] += Acc_ext[i];
        acceleration[i] += gravity[i];
        acceleration[i] += (F_press[i] + F_vis[i] + F_surface[i]) / density;
    }
}

void Particle::update_position(float tstep)
{
    dt = tstep;
    for (int i = 0; i != 3; ++i)
    {
        velocity[i] += acceleration[i] * dt;
        position[i] += velocity[i] * dt;
        assert(!isnan(position[i]));
        position[i] = clamp(position[i], -1.0f, 1.0f); //FIXME
    }
    update_bucket();
}

float Particle::Z_wall_dens(const float distance)
{
    if (distance >= effective_radius)
    //~ if (distance >= r_0)
        return 0.0f;
    //~ dump(wall_dens_vec[static_cast<int>((1.0f - distance / 0.5f) * wall_dens_vec.size() - 1)]);
    return wall_dens_vec[static_cast<int>((1.0f - distance / 0.5f) * wall_dens_vec.size() - 1)];
}

float Particle::Z_wall_vis(const float distance)
{
    if (distance >= effective_radius)
        return 0.0f;
    return wall_vis_vec[static_cast<int>((1.0f - distance / 0.5f) * wall_vis_vec.size() - 1)];
}

void Particle::init_wall()
{
    //adding wall particles
    Buckets wallbucket(1,1,1);
    ParticlesBase wallparticles;
    for (float x = -10.0f*r_0; x <= 10.0f*r_0; x += 2*r_0 - eps)
        for (float y = -10.0f*r_0; y <= 10.0f*r_0; y += 2*r_0 - eps)
            for (float z = -10.0f*r_0; z <= 0.0f; z += 2*r_0 - eps)
            {
                float pos[3] = {x, y, z};
                wallparticles.push_back(Particle(pos, &wallbucket));
                //~ wallparticles.back().mass = 0.1f;
                wallparticles.back().density = wallparticles.back().mass / mass_0 * wallparticles.back().density;
            }
    float pos[3] = {0.0f, 0.0f, 0.5f};
    wallparticles.push_back(Particle(pos, &wallbucket));

    Particle &last_particle = wallparticles.back();
    last_particle.velocity[0] = 1.0f;

    float nullmeasure = last_particle.update_density();
    for (float z = 0.5f; z >= 0.0f;  z -= 0.005f - eps)
    {
        last_particle.position[2] = z;
        wall_dens_vec.push_back(last_particle.update_density() - nullmeasure);
        //~ wall_dens_vec.push_back(2000000.0f);
        last_particle.update_forces();
        wall_vis_vec.push_back(last_particle.F_vis[0]);
    }
}

void Particle::update_press()
{
    pressure = pressure_0 + stiffness * (density - density_0);
}

ParticlesBase::ParticlesBase()
:
    std::vector<Particle>()
{}

ParticlesBase::~ParticlesBase()
{
}

//~ void ParticlesBase::update(float tstep)
//~ {   
    //~ for (Particles::iterator it = begin(); it != end(); ++it)
        //~ it->update_density(tstep);
    //~ for (Particles::iterator it = begin(); it != end(); ++it)
        //~ it->update(tstep);
    //~ for (Particles::iterator it = begin(); it != end(); ++it)
    //~ {
        //~ it->update_position(tstep);
        //~ if (twod_enabled)
        //~ {
            //~ it->position[2] = 0.0f;
            //~ it->velocity[2] = 0.0f;
        //~ }
    //~ }
        //~ 
//~ }

void ParticlesBase::update(float tstep)
{
    size_t it = 0;

    #pragma omp parallel for
    for (it = 0; it < size(); ++it)
        at(it).update_density(tstep);

    #pragma omp barrier

    #pragma omp parallel for
    for (it = 0; it < size(); ++it)
        at(it).update(tstep);

    #pragma omp barrier

    for (it = 0; it < size(); ++it)
        at(it).update_position(tstep);
}

}
