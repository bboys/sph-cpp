#include "plane.h"
#include <iostream>
#include <cstring>
#include <cassert>

#define GL_GLEXT_PROTOTYPES

#include <GL/glext.h>
#include <GL/glut.h>

#include "shader.h"

namespace Graphics {

namespace Cg {

using namespace Base;

Planes::~Planes()
{
    if (vbo_id)
        glDeleteBuffersARB(1, &vbo_id);
}

void Planes::draw(size_t index)
{
    shader.bind_program(ShaderType::VERTEX);
    if (!shader.bind_program(ShaderType::FRAGMENT, index)) return;

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo_id);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Plane), 0);

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, sizeof(Plane), reinterpret_cast<void *>(sizeof(float)*3));

    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(3, GL_FLOAT, sizeof(Plane), reinterpret_cast<void *>(sizeof(float)*7));

    glDrawArrays(GL_POINTS, 0, size());

    glClientActiveTexture(GL_TEXTURE0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glActiveTexture(GL_TEXTURE0);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

    shader.unbind();
}

void Planes::draw_no_vbo(size_t index)
{
    shader.bind_program(ShaderType::VERTEX);
    if (!shader.bind_program(ShaderType::FRAGMENT, index)) return;

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(Plane), &(*this)[0].position);

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, sizeof(Plane), &(*this)[0].color);

    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(3, GL_FLOAT, sizeof(Plane), &(*this)[0].direction);

    glDrawArrays(GL_POINTS, 0, size());

    glClientActiveTexture(GL_TEXTURE0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glActiveTexture(GL_TEXTURE0);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    shader.unbind();
}

void Planes::init_vbo()
{
    glGenBuffersARB(1, &vbo_id);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo_id);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, size()*sizeof(Plane), &(*this)[0].position, GL_STATIC_DRAW_ARB);
}

void Planes::init_shader()
{
    shader.init();

    shader.load_program(ShaderType::VERTEX, "plane_v.cg");
    shader.load_program(ShaderType::FRAGMENT, "plane_f.cg");
    shader.load_program(ShaderType::FRAGMENT, "plane_shadows_f.cg");

    Objects::init_shader();
}

void Planes::init_old_shader()
{
    shader.init();

    shader.load_program(ShaderType::VERTEX, "plane_v.cg");
    shader.load_program(ShaderType::FRAGMENT, "plane_old_f.cg");

    Objects::init_shader();
}

}

}
