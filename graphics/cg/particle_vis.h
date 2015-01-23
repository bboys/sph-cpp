#ifndef PARTICLE_VIS_H_ICS
#define PARTICLE_VIS_H_ICS

#include "particle.h"
#include <vector>

class Particles : public ParticlesBase
{
    bool twod_enabled;
public:
    Particles();
    virtual ~Particles();
    void update(float tstep);
    void draw(size_t index = 0);
    void draw_opengl();
    void draw_no_vbo(size_t index = 0);
    void init_shader();
    void init_old_shader();
    void init_water_shader();
    void init_vbo();
    void toggle_2d_mode();
};

#endif
