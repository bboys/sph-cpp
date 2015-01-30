#include "particle_vis.h"

#define GL_GLEXT_PROTOTYPES

#include <GL/glext.h>
#include <GL/glut.h>

#include "shader.h"

namespace Graphics {

namespace Cg {

Particles::~Particles()
{
}

void Particles::draw(size_t index)
{
    draw_no_vbo();
}

void Particles::draw_no_vbo(size_t index)
{
    shader.bind_program(ShaderType::VERTEX);
    if (!shader.bind_program(ShaderType::FRAGMENT, index)) return;

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Physics::Particle), &(*particles)[0].position);

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, sizeof(Graphics::Base::Particle), &(*this)[0].color);

    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(1, GL_FLOAT, sizeof(Physics::Particle), &(*particles)[0].radius);

    glClientActiveTexture(GL_TEXTURE1);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(3, GL_FLOAT, sizeof(Physics::Particle), &(*particles)[0].normal);

    glDrawArrays(GL_POINTS, 0, particles->size());

    glClientActiveTexture(GL_TEXTURE1);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glClientActiveTexture(GL_TEXTURE0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    shader.unbind();
}

void Particles::init_shader()
{
    shader.init();

    glDisable(GL_BLEND);

    shader.load_program(ShaderType::VERTEX, "sphere_v.cg");
    shader.load_program(ShaderType::FRAGMENT, "sphere_f.cg");
    shader.load_program(ShaderType::FRAGMENT, "sphere_shadows_f.cg");

    Objects::init_shader();
}

void Particles::init_old_shader()
{
    shader.init();
    shader.load_program(ShaderType::VERTEX, "sphere_v.cg");
    shader.load_program(ShaderType::FRAGMENT, "sphere_old_f.cg");

    Objects::init_shader();
}

void Particles::init_water_shader()
{
    shader.init();

    shader.load_program(ShaderType::VERTEX, "particle_v.cg");
    shader.load_program(ShaderType::FRAGMENT, "particle_f.cg");

    Objects::init_shader();
}

}

}
