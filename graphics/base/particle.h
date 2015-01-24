#ifndef SPH_GRAPHICS_BASE_PARTICLE_H
#define SPH_GRAPHICS_BASE_PARTICLE_H

#include "../../particle.h"

namespace Graphics {

namespace Base {

class Particles : public ParticlesBase
{
    bool twod_enabled;
public:
    Particles(): ParticlesBase(), twod_enabled(false) {};
    virtual ~Particles() {};

    void update(float tstep)
    {
        ParticlesBase::update(tstep);

        for (size_t it = 0; it < size(); ++it)
            if (twod_enabled)
            {
                at(it).position[2] = 0.0f;
                at(it).velocity[2] = 0.0f;
            }
    };

    virtual void draw(size_t index = 0) = 0;
    virtual void draw_opengl() = 0;
    virtual void draw_no_vbo(size_t index = 0) = 0;
    virtual void init_shader() = 0;
    virtual void init_old_shader() = 0;
    virtual void init_water_shader() = 0;
    void toggle_2d_mode() {twod_enabled = !twod_enabled;};
};

}

}

#endif
