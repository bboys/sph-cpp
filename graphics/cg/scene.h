#ifndef SPH_GRAPHICS_CG_SCENE_H
#define SPH_GRAPHICS_CG_SCENE_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <memory>

#include "graphics/base/scene.h"
#include "object.h"
#include "shader.h"

namespace Graphics {

namespace Cg {

class Scene : public Base::Scene
{
    GLuint fbo_array[3];
    const GLenum buffers[5];
    GLuint tex_array[5];
    Graphics::Cg::Shader shader;
    GLuint renderbuffer_array[3];
    Graphics::Cg::ParameterMap parameters;
public:
    Scene();
    Scene(int width, int height);
    Scene(Base::Scene const &other);
    ~Scene() {};
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
private:
    void set_parameters();
    void set_parameters(Graphics::Cg::Objects &objects);
    bool check_framebuffer_status() const;
};

}

}

#endif
