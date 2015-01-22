#ifndef PLANE_H_ICS
#define PLANE_H_ICS

#include "object.h"

class Plane : public Object
{
public:
    float direction[3];

    Plane(float position[3], float color[4], float direction[3]);
};

class Planes : public Objects<Plane>
{
    GLuint vbo_id;
public:
    ~Planes();
    void draw(size_t index = 0);
    void draw_opengl();
    void draw_no_vbo(size_t index = 0);
    void init_shader();
    void init_old_shader();
    void init_vbo();
};

#endif
