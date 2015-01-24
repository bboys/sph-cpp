#ifndef SPH_GRAPHICS_OPENGL_PARTICLE_H
#define SPH_GRAPHICS_OPENGL_PARTICLE_H

#include "graphics/base/particle.h"

namespace Graphics {

namespace OpenGL {

class Particles : public Base::Particles
{
public:
    Particles();
    Particles(Base::Particles const &other): Base::Particles(other){};
    Particles(Particles &&) = default;
    virtual ~Particles();
    void draw(size_t index = 0);
    void draw_opengl();
    void draw_no_vbo(size_t index = 0);
    void init_shader();
    void init_old_shader();
    void init_water_shader();
};

}

}

#endif
