#ifndef SCENE_H_ICS
#define SCENE_H_ICS

#include <GL/glew.h>
#include <GL/glut.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <vector>
#include "graphics/base/particle.h"
#include "graphics/base/plane.h"
#include "particle_vis.h"
#include "plane.h"
#include "shader.h"
#include <memory>
#include <map>

enum class GraphicsType
{
    BASE,
    CG,
    OPENGL
};

class Scene
{
    GLdouble near;
    GLdouble far;
    GLdouble top;
    GLdouble bottom;
    GLdouble right;
    GLdouble left;

public:
    std::shared_ptr<Graphics::Base::Particles> particles;
    std::shared_ptr<Graphics::Base::Planes> planes;

private:
    std::map<GraphicsType, std::shared_ptr<Graphics::Base::Particles> > particles_map;
    std::map<GraphicsType, std::shared_ptr<Graphics::Base::Planes> > planes_map;

    GLuint fbo_array[3];
    bool opengl_enabled;
    bool deferred_enabled;
    bool vbo_enabled;
    bool normals_enabled;
    bool shadows_enabled;
    bool contour_enabled;
    bool water_enabled;
    const GLenum buffers[5];
    GLuint tex_array[5];
    Graphics::Cg::Shader shader;
    GLuint renderbuffer_array[3];
    float epsilon;
    int width;
    int height;
    ParameterMap parameters;
    float pmatrix[16];
public:
    Scene();
    Scene(int width, int height);
    void set_perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
    void draw_particles_from_shader(size_t index = 0);
    void draw_particles();
    void draw_particles_depth_only();
    void draw_particles_color_only();
    void draw_planes_from_shader(size_t index = 0);
    void draw_planes();
    void draw_normpass();
    void draw_shadow_map();
    void draw_fullscreen_quad();
    void draw_contours();
    void reshape();
    void init_shaders();
    void bind_fbo(size_t index = 0);
    void save_to_png(std::string const &filename);
    void toggle_opengl();
    void toggle_deferred();
    void toggle_vbo();
    void toggle_normals();
    void toggle_shadows();
    void toggle_contour();
    void toggle_water();
    void set_size(int w, int h);
    void add_graphics_type_poiners();
private:
    void set_parameters();
    template <class T>
    void set_parameters(Objects<T> &objects);
    bool check_framebuffer_status() const;
};

#endif
