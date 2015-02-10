#include "graphics/opengl/particle.h"

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

namespace Graphics {

namespace OpenGL {

Particles::~Particles()
{
}

void Particles::draw(size_t index)
{
    std::vector<Base::Particle>::iterator it = begin();
    std::vector<Physics::Particle>::iterator it2 = particles->begin();
    for ( ;it != end() and it2 != particles->end();++it, ++it2)
    {
        glPushMatrix();
        glColor4fv(it->color);
        glTranslatef(it2->position[0], it2->position[1], it2->position[2]);
        glutSolidSphere(it2->radius, 8, 8);
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
