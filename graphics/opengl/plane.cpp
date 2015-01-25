#include "graphics/opengl/plane.h"

#define GL_GLEXT_PROTOTYPES

#include <GL/glext.h>
#include <GL/glut.h>

namespace Graphics {

namespace OpenGL {

Planes::~Planes()
{
}

void Planes::draw(size_t index)
{
    for (std::vector<Base::Plane>::iterator it = this->begin(); it != this->end(); ++it)
    {
        glPushMatrix();
        glColor4fv(it->color);
        glPopMatrix();
    }
}

void Planes::draw_no_vbo(size_t index)
{
    draw(index);
}

void Planes::init_shader()
{
}

void Planes::init_old_shader()
{
}

void Planes::init_vbo()
{
}

}

}
