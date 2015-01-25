#ifndef SPH_GRAPHICS_OPENGL_PLANE_H
#define SPH_GRAPHICS_OPENGL_PLANE_H

#include "graphics/base/plane.h"

namespace Graphics {

namespace OpenGL {

class Planes : public Base::Planes
{
public:
    Planes() = default;
    Planes(Base::Planes const &other): Base::Planes(other){};
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
