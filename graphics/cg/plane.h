#ifndef SPH_GRAPHICS_CG_PLANE_H
#define SPH_GRAPHICS_CG_PLANE_H

#include "graphics/base/plane.h"
#include "object.h"

namespace Graphics {

namespace Cg {

class Planes : public Base::Planes, public Cg::Objects
{
public:
    Planes() = default;
    Planes(Base::Planes const &other): Base::Planes(other), Cg::Objects() {};
    virtual ~Planes();
    void draw(size_t index = 0);
    void draw_opengl();
    void draw_no_vbo(size_t index = 0);
    void init_shader();
    void init_old_shader();
    void init_vbo();
};

}

}

#endif
