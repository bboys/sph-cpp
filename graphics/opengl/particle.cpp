#include "graphics/opengl/particle.h"

#define GL_GLEXT_PROTOTYPES

#include <GL/glext.h>
#include <GL/glut.h>

namespace Graphics {

namespace OpenGL {

Particles::Particles()
:
    Base::Particles()
{}

Particles::~Particles()
{
}

void Particles::draw(size_t index)
{
    for (std::vector<Particle>::iterator it = particles->begin(); it != particles->end(); ++it)
    {
        glPushMatrix();
        glColor4fv(it->color);
        glTranslatef(it->position[0], it->position[1], it->position[2]);
        glutSolidSphere(it->radius, 8, 8);
        glPopMatrix();
    }
}

void Particles::draw_no_vbo(size_t index)
{
    draw(index);
}

void Particles::init_shader()
{
}

void Particles::init_old_shader()
{
}

void Particles::init_water_shader()
{
}

}

}
