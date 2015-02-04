#include "scene.h"
#include <math.h>
#include <cassert>
#include <cstring>
#include "shader.h"
#include "plane.h"
#include "particle.h"


namespace Graphics {

namespace Cg {

void Scene::set_parameters()
{
    ParameterList *list;

    float pos[4];
    glGetLightfv(GL_LIGHT0, GL_POSITION, pos);
    list = &parameters["lightpos"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetParameter4fv(*it, pos);

    list = &parameters["color"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetTextureParameter(*it, tex_array[0]);

    list = &parameters["normal"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetTextureParameter(*it, tex_array[1]);

    list = &parameters["density"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetTextureParameter(*it, tex_array[1]);

    list = &parameters["pos"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetTextureParameter(*it, tex_array[2]);

    list = &parameters["shadowmap"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetTextureParameter(*it, tex_array[3]);

    list = &parameters["shadedcolor"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetTextureParameter(*it, tex_array[4]);

    list = &parameters["ShadowProj"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetMatrixParameterfc(*it, pmatrix);

    list = &parameters["texturesize"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetParameter2f(*it, (float)width, (float)height);
}

void Scene::set_parameters(Objects &objects)
{
    ParameterList *list;
    list = &objects.parameters["ModelViewProj"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetStateMatrixParameter(*it, CG_GL_MODELVIEW_PROJECTION_MATRIX,
                                    CG_GL_MATRIX_IDENTITY);

    list = &objects.parameters["ModelViewI"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetStateMatrixParameter(*it, CG_GL_MODELVIEW_MATRIX,
                                    CG_GL_MATRIX_INVERSE);

    list = &objects.parameters["ProjI"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetStateMatrixParameter(*it, CG_GL_PROJECTION_MATRIX,
                                CG_GL_MATRIX_INVERSE);

    list = &objects.parameters["ModelView"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetStateMatrixParameter(*it, CG_GL_MODELVIEW_MATRIX,
                                CG_GL_MATRIX_IDENTITY);

    list = &objects.parameters["ModelViewIT"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetStateMatrixParameter(*it, CG_GL_MODELVIEW_MATRIX,
                                CG_GL_MATRIX_INVERSE_TRANSPOSE);

    list = &objects.parameters["near"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetParameter1f(*it, near);

    list = &objects.parameters["top"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetParameter1f(*it, top);

    list = &objects.parameters["bottom"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetParameter1f(*it, bottom);

    list = &objects.parameters["epsilon"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetParameter1f(*it, epsilon);

    list = &objects.parameters["unproj_scale"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetParameter2f(*it, (right-left)/width, (top-bottom)/height);

    list = &objects.parameters["unproj_offset"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
            cgGLSetParameter2f(*it, (right-left)/2.0f, (top-bottom)/2.0f);

    list = &objects.parameters["zb_scale"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetParameter1f(*it, far*near/(far-near));

    list = &objects.parameters["zb_offset"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetParameter1f(*it, far/(far-near));

    list = &objects.parameters["wsize"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetParameter2f(*it, (float)width, (float)height);

    float pos[4];
    glGetLightfv(GL_LIGHT0, GL_POSITION, pos);
    list = &objects.parameters["lightpos"];
    for (ParameterList::iterator it = list->begin(); it != list->end(); ++it)
        cgGLSetParameter4fv(*it, pos);
}

void Scene::draw_particles()
{
    draw_particles_from_shader();
}

void Scene::draw_particles_depth_only()
{
    glDisable(GL_BLEND);
    if (!water_enabled)
        return;

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_TRUE);

    epsilon = 0.5f;

    draw_particles();
}

void Scene::draw_particles_color_only()
{
    if (water_enabled)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);

        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_FALSE);

        epsilon = 0.0f;
    }
    draw_particles();

    glDepthMask(GL_TRUE);
}

void Scene::draw_particles_from_shader(size_t index)
{
    set_parameters(*reinterpret_cast<Particles*>(particles.get()));
    //~ set_parameters(*std::dynamic_pointer_cast<Particles>(particles));
    if (vbo_enabled)
        particles->draw(index);
    else
        particles->draw_no_vbo(index);
}

void Scene::draw_planes()
{
    draw_planes_from_shader();
}

void Scene::draw_planes_from_shader(size_t index)
{
    set_parameters(*reinterpret_cast<Planes*>(planes.get()));
    //~ set_parameters(*std::dynamic_pointer_cast<Planes>(planes));
    if (vbo_enabled)
        planes->draw(index);
    else
        planes->draw_no_vbo(index);
}

void Scene::draw_shadow_map()
{
    if (!deferred_enabled || !shadows_enabled)
        return;

    draw_particles_from_shader(1);
}

void Scene::draw_normpass()
{
    if (!deferred_enabled)
        return;

    if (shadows_enabled)
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glMultMatrixf(pmatrix);
        glGetFloatv(GL_MODELVIEW_MATRIX, pmatrix);

        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();

        glMultMatrixf(pmatrix);
        glGetFloatv(GL_PROJECTION_MATRIX, pmatrix);

        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

    set_parameters();

    size_t shader_index = 0;
    if (!shadows_enabled)
        shader_index = 2;
    if (normals_enabled)
        shader_index = 1;

    shader.bind_program(Graphics::ShaderType::FRAGMENT, shader_index);

    draw_fullscreen_quad();

    shader.unbind();
}

void Scene::draw_contours()
{
    if (!deferred_enabled)
        return;

    if (contour_enabled)
    {
        set_parameters();
        shader.bind_program(Graphics::ShaderType::FRAGMENT, 3);
    }
    else
    {
        glDisable(GL_LIGHTING);
        glColor3f(1,1,1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex_array[4]);
    }

    draw_fullscreen_quad();

    if (contour_enabled)
        shader.unbind();
    else
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
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

bool Scene::check_framebuffer_status() const
{
    // check FBO status
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
        std::cout << "Framebuffer complete." << std::endl;
        return true;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
        return false;

    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Unsupported by FBO implementation." << std::endl;
        return false;

    default:
        std::cout << "[ERROR] Framebuffer incomplete: Unknown error." << std::endl;
        return false;
    }
}

void Scene::reshape()
{
    //~ static const int shadow_map_size = 512;
    shader.init();

    shader.load_program(Graphics::ShaderType::FRAGMENT, "shading_f.cg");
    shader.load_program(Graphics::ShaderType::FRAGMENT, "normals_f.cg");
    shader.load_program(Graphics::ShaderType::FRAGMENT, "deferred_f.cg");
    shader.load_program(Graphics::ShaderType::FRAGMENT, "contour_f.cg");
    shader.bind_program(Graphics::ShaderType::FRAGMENT);

    parameters.clear();

    parameters.insert(ParameterPair("lightpos", shader.get_parameters("lightpos")));
    parameters.insert(ParameterPair("color", shader.get_parameters("color")));
    parameters.insert(ParameterPair("normal", shader.get_parameters("normal")));
    parameters.insert(ParameterPair("pos", shader.get_parameters("pos")));
    parameters.insert(ParameterPair("shadowmap", shader.get_parameters("shadowmap")));
    parameters.insert(ParameterPair("ShadowProj", shader.get_parameters("ShadowProj")));
    parameters.insert(ParameterPair("shadedcolor", shader.get_parameters("shadedcolor")));
    parameters.insert(ParameterPair("texturesize", shader.get_parameters("texturesize")));
    parameters.insert(ParameterPair("density", shader.get_parameters("density")));

    glDeleteFramebuffersEXT(3, fbo_array);
    glDeleteRenderbuffersEXT(3, renderbuffer_array);
    glDeleteTextures(5, tex_array);

    glGenTextures(5, tex_array);
    glBindTexture(GL_TEXTURE_2D, tex_array[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, width, height, 0, GL_RGBA,
      GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, tex_array[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, width, height, 0, GL_RGBA,
      GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, tex_array[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, width, height, 0, GL_RGBA,
      GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, tex_array[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, height, 0, GL_RED,
      GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glBindTexture(GL_TEXTURE_2D, tex_array[4]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, width, height, 0, GL_RGBA,
      GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffersEXT(2, renderbuffer_array);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderbuffer_array[0]);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
      width, height);

    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderbuffer_array[1]);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
      width, height);

    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

    glGenFramebuffersEXT(3, fbo_array);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_array[0]);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
      GL_TEXTURE_2D, tex_array[0], 0);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
      GL_TEXTURE_2D, tex_array[1], 0);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT,
      GL_TEXTURE_2D, tex_array[2], 0);

    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
      GL_RENDERBUFFER_EXT, renderbuffer_array[0]);

    assert(check_framebuffer_status());

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_array[1]);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
      GL_TEXTURE_2D, tex_array[3], 0);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
      GL_RENDERBUFFER_EXT, renderbuffer_array[1]);

    assert(check_framebuffer_status());

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_array[2]);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
      GL_TEXTURE_2D, tex_array[4], 0);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
      GL_RENDERBUFFER_EXT, renderbuffer_array[2]);

    assert(check_framebuffer_status());

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    shader.unbind();
}

void Scene::init_shaders()
{
    if (water_enabled)
        particles->init_water_shader();
    else if (deferred_enabled)
        particles->init_shader();
    else
        particles->init_old_shader();

    if (deferred_enabled)
        planes->init_shader();
    else
        planes->init_old_shader();
    if (vbo_enabled)
        planes->init_vbo();
}

void Scene::bind_fbo(size_t index)
{
    if (!deferred_enabled)
        return;

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_array[index]);

    glDrawBuffers((index == 0 ? 3 : 1), &buffers[(index == 0 ? 0 : (2 + index))]);

    glClearColor(0.0f, 0.0f, 0.0f, 0.00001f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (index == 1)
    {
        GLfloat mvmatrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, mvmatrix);

        float mvmatrixinv[16] = {mvmatrix[0], mvmatrix[4], mvmatrix[8], mvmatrix[3],
                           mvmatrix[1], mvmatrix[5], mvmatrix[9], mvmatrix[7],
                           mvmatrix[2], mvmatrix[6], mvmatrix[10], mvmatrix[11],
                           -(mvmatrix[0] * mvmatrix[12] + mvmatrix[1] * mvmatrix[13] + mvmatrix[2] * mvmatrix[14]),
                           -(mvmatrix[4] * mvmatrix[12] + mvmatrix[5] * mvmatrix[13] + mvmatrix[6] * mvmatrix[14]),
                           -(mvmatrix[8] * mvmatrix[12] + mvmatrix[9] * mvmatrix[13] + mvmatrix[10] * mvmatrix[14]),
                           mvmatrix[15]};

        memcpy(pmatrix, mvmatrixinv, sizeof(float) * 16);
    }
}

void Scene::save_to_png(std::string const &filename)
{
    //~ std::vector<unsigned char> tex_data, png_data;
    //~ tex_data.resize(width * height * 4);
//~ 
    //~ glPixelStorei(GL_PACK_ALIGNMENT, 1);
    //~ glReadBuffer(GL_FRONT);
    //~ glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &tex_data[0]);
    //~ std::vector<unsigned char>::iterator it_begin = tex_data.begin();
    //~ std::vector<unsigned char>::iterator it_end = tex_data.end() - width*4;
    //~ for (size_t i = 0; i < height / 2; ++i)
    //~ {
        //~ std::swap_ranges(it_begin, it_begin+width*4, it_end);
        //~ it_begin += width*4;
        //~ it_end -= width*4;
    //~ }
    //~ LodePNG::Encoder encoder;
    //~ encoder.encode(png_data, tex_data, width, height);
    //~ LodePNG::saveFile(png_data, filename);
    std::cout << "Not implemented" << std::endl;
}

Scene::Scene()
:
    Base::Scene(std::make_shared<Particles>(), std::make_shared<Planes>(), 0, 0),
    fbo_array(),
    buffers{GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT0_EXT},
    tex_array(),
    renderbuffer_array()
{
}

Scene::Scene(int width, int height)
:
    Base::Scene(std::make_shared<Particles>(), std::make_shared<Planes>(), width, height),
    fbo_array(),
    buffers{GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT0_EXT},
    tex_array(),
    renderbuffer_array()
{
}

Scene::Scene(Base::Scene const &other)
:
    Base::Scene(other),
    fbo_array(),
    buffers{GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT0_EXT},
    tex_array(),
    renderbuffer_array()
{
    particles = std::make_shared<Particles>(*other.particles);
    planes = std::make_shared<Planes>(*other.planes);
}

}

}
