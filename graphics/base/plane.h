#ifndef SPH_GRAPHICS_BASE_PLANE_H
#define SPH_GRAPHICS_BASE_PLANE_H

#include "object.h"
#include <cstring>

namespace Graphics {

namespace Base {

class Plane : public Object
{
public:
    float direction[3];

    Plane(float position[3], float color[4], float dir[3])
    :
        Object(position, color)
    {
        memcpy(direction, dir, sizeof(float)*3);
    };
};

class Planes : public Objects<Plane>
{
protected:
    GLuint vbo_id;
public:
    Planes (): Objects(), vbo_id(0) {};
    Planes (Planes const &other): Objects(other), vbo_id(other.vbo_id) {};
    virtual ~Planes() {};
    virtual void draw(size_t index = 0) = 0;
    virtual void draw_no_vbo(size_t index = 0) = 0;
    virtual void init_shader() = 0;
    virtual void init_old_shader() = 0;
    virtual void init_vbo() = 0;
};

}

}

#endif
