#ifndef SPHERE_H_ICS
#define SPHERE_H_ICS

#include "object.h"

class Sphere : public Object
{
public:
    float radius;
    Sphere(float position[3], float color[4]) : Object(position, color), radius(0.05) { }
    Sphere(float position[3], float color[4], float radius) : Object(position, color), radius(radius) { }
};

class Spheres : public Objects<Sphere>
{
    GLuint vbo_id;
public:
    ~Spheres();
    void draw(size_t index = 0);
    void draw_opengl();
    void draw_no_vbo(size_t index = 0);
    void init_shader();
    void init_old_shader();
    void init_vbo();
};

#endif
