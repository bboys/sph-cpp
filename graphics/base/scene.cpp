#include "scene.h"

namespace Graphics {

namespace Base {

void Scene::set_perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    top = zNear * tan(fovy * M_PI / 360.0);
    bottom = -top;
    right = top * aspect;
    left = -right;
    far = zFar;
    near = zNear;
    glFrustum(left, right, bottom, top, zNear, zFar);
}

void Scene::draw_particles()
{
    particles->draw();
}

void Scene::draw_particles_depth_only()
{
}

void Scene::draw_particles_color_only()
{
    draw_particles();
}

void Scene::draw_planes()
{
    planes->draw();
}

void Scene::draw_shadow_map()
{
}

void Scene::draw_normpass()
{
}

void Scene::draw_contours()
{
}

void Scene::draw_fullscreen_quad()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);

    glTexCoord2i(0, 0);
    glVertex2i(0, 0);
    glTexCoord2i(0, 1);
    glVertex2i(0, height);
    glTexCoord2i(1, 1);
    glVertex2i(width, height);
    glTexCoord2i(1, 0);
    glVertex2i(width, 0);

    glEnd();

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

void Scene::reshape()
{
}

void Scene::init_shaders()
{
}

void Scene::bind_fbo(size_t index)
{
}

void Scene::save_to_png(std::string const &filename)
{
}

void Scene::toggle_normals()
{
    normals_enabled = !normals_enabled;
}

void Scene::toggle_deferred()
{
    deferred_enabled = !deferred_enabled;
    init_shaders();
}

void Scene::toggle_vbo()
{
    vbo_enabled = !vbo_enabled;
}

void Scene::toggle_shadows()
{
    shadows_enabled = !shadows_enabled;
}

void Scene::toggle_contour()
{
    contour_enabled = !contour_enabled; 
}

void Scene::toggle_water()
{
    water_enabled = !water_enabled;
    init_shaders();
}

void Scene::set_size(int w, int h)
{
    width = w;
    height = h;
}

Scene::Scene(std::shared_ptr<Particles> _particles, std::shared_ptr<Planes> _planes, int width, int height)
:
    particles(_particles),
    planes(_planes),
    deferred_enabled(false),
    vbo_enabled(true),
    normals_enabled(false),
    shadows_enabled(false),
    contour_enabled(false),
    water_enabled(false),
    epsilon(0.0f),
    width(width),
    height(height)
{
}

}

}
