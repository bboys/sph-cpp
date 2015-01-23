#include "sphere.h"
#include <iostream>

#define GL_GLEXT_PROTOTYPES

#include <GL/glext.h>
#include <GL/glut.h>

#include "shader.h"

Spheres::~Spheres()
{
    if (vbo_id)
        glDeleteBuffersARB(1, &vbo_id);
}

void Spheres::draw_opengl()
{
    for (std::vector<Sphere>::iterator it = this->begin(); it != this->end(); ++it)
    {
        glPushMatrix();
        glColor4fv(it->color);
        glTranslatef(it->position[0], it->position[1], it->position[2]);
        glutSolidSphere(it->radius, 8, 8);
        glPopMatrix();
    }
}

void Spheres::draw(size_t index)
{
    shader.bind_program(ShaderType::VERTEX);
    if (!shader.bind_program(ShaderType::FRAGMENT, index)) return;

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo_id);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Sphere), 0);

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, sizeof(Sphere), reinterpret_cast<void *>(sizeof(float)*3));

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(1, GL_FLOAT, sizeof(Sphere), reinterpret_cast<void *>(sizeof(float)*7));

    glDrawArrays(GL_POINTS, 0, size());

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

    shader.unbind();
}

void Spheres::draw_no_vbo(size_t index)
{
    shader.bind_program(ShaderType::VERTEX);
    if (!shader.bind_program(ShaderType::FRAGMENT, index)) return;

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(Sphere), &(*this)[0].position);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glTexCoordPointer(1, GL_FLOAT, sizeof(Sphere), &(*this)[0].radius);

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, sizeof(Sphere), &(*this)[0].color);

    glDrawArrays(GL_POINTS, 0, size());

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    shader.unbind();
}

void Spheres::init_vbo()
{
    glGenBuffersARB(1, &vbo_id);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo_id);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, size()*sizeof(Sphere), &(*this)[0].position, GL_STATIC_DRAW_ARB);
}

void Spheres::init_shader()
{
    shader.init();

    shader.load_program(ShaderType::VERTEX, "sphere_v.cg");
    shader.load_program(ShaderType::FRAGMENT, "sphere_f.cg");
    shader.load_program(ShaderType::FRAGMENT, "sphere_shadows_f.cg");

    Objects<Sphere>::init_shader();
}

void Spheres::init_old_shader()
{
    shader.init();

    shader.load_program(ShaderType::VERTEX, "sphere_v.cg");
    shader.load_program(ShaderType::FRAGMENT, "sphere_old_f.cg");

    Objects<Sphere>::init_shader();
}
