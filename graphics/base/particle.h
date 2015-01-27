#ifndef SPH_GRAPHICS_BASE_PARTICLE_H
#define SPH_GRAPHICS_BASE_PARTICLE_H

#include "../../particle.h"
#include <memory>
#include "object.h"

namespace Graphics {

namespace Base {

class Particle : public Object
{
public:
    Particle(Particle const &other)
    :
        Object(other)
    {};

    Particle(float position[3], float color[4])
    :
        Object(position, color)
    {};

    Particle(Physics::Particle &particle, float color[4])
    :
        Object(particle.position, color)
    {};
};

class Particles : public Objects<Particle>
{
protected:
    std::shared_ptr<Physics::ParticlesBase> particles;
private:
    bool twod_enabled;
public:
    Particles(): Objects(), particles(new Physics::ParticlesBase()), twod_enabled(false) {};
    Particles(Particles const &other): Objects(other), particles(other.particles), twod_enabled(other.twod_enabled) {};
    virtual ~Particles() {};

    void update(float tstep)
    {
        particles->update(tstep);

        for (size_t it = 0; it < particles->size(); ++it)
            if (twod_enabled)
            {
                particles->at(it).position[2] = 0.0f;
                particles->at(it).velocity[2] = 0.0f;
            }
    };

    virtual void draw(size_t index = 0) = 0;
    virtual void draw_no_vbo(size_t index = 0) = 0;
    virtual void init_shader() = 0;
    virtual void init_old_shader() = 0;
    virtual void init_water_shader() = 0;
    void toggle_2d_mode() {twod_enabled = !twod_enabled;};

    void set_particles(std::shared_ptr<Physics::ParticlesBase> new_particles) {particles = new_particles;};
    std::shared_ptr<Physics::ParticlesBase> get_particles() {return particles;};
};

}

}

#endif
