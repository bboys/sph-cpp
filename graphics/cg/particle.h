#ifndef PARTICLE_VIS_H_ICS
#define PARTICLE_VIS_H_ICS

#include "graphics/base/particle.h"
#include "object.h"

namespace Graphics {

namespace Cg {

class Particles : public Base::Particles, public Cg::Objects
{
public:
    Particles() = default;
    Particles(Base::Particles const &other): Base::Particles(other), Cg::Objects() {};
    virtual ~Particles();
    void draw(size_t index = 0);
    void draw_no_vbo(size_t index = 0);
    void init_shader();
    void init_old_shader();
    void init_water_shader();
};

}

}

#endif
