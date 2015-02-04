#ifndef SPH_GRAPHICS_BASE_SCENE_H
#define SPH_GRAPHICS_BASE_SCENE_H

#include <GL/glew.h>
#include <GL/glut.h>
#include "particle.h"
#include "plane.h"
#include <memory>

namespace Graphics {

namespace Base {

class Scene
{
protected:
    GLdouble near;
    GLdouble far;
    GLdouble top;
    GLdouble bottom;
    GLdouble right;
    GLdouble left;

public:
    std::shared_ptr<Graphics::Base::Particles> particles;
    std::shared_ptr<Graphics::Base::Planes> planes;

protected:
    bool deferred_enabled;
    bool vbo_enabled;
    bool normals_enabled;
    bool shadows_enabled;
    bool contour_enabled;
    bool water_enabled;
    float epsilon;
    int width;
    int height;
    float pmatrix[16];
public:
    Scene(std::shared_ptr<Particles> _particles, std::shared_ptr<Planes> _planes, int width, int height);
    Scene(Scene const &other) = default;
    virtual ~Scene() {};
    virtual void set_perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
    virtual void draw_particles();
    virtual void draw_particles_depth_only();
    virtual void draw_particles_color_only();
    virtual void draw_planes();
    virtual void draw_normpass();
    virtual void draw_shadow_map();
    virtual void draw_fullscreen_quad();
    virtual void draw_contours();
    virtual void reshape();
    virtual void init_shaders();
    virtual void bind_fbo(size_t index = 0);
    virtual void save_to_png(std::string const &filename);
    void toggle_deferred();
    void toggle_vbo();
    void toggle_normals();
    void toggle_shadows();
    void toggle_contour();
    void toggle_water();
    void set_size(int w, int h);
};

}

}

#endif
