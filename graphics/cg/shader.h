#ifndef SPH_GRAPHICS_CG_SHADER_H
#define SPH_GRAPHICS_CG_SHADER_H

#define GL_GLEXT_PROTOTYPES

#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <string>
#include <iostream>
#include <vector>

#include "graphics/base/shader.h"

namespace Graphics {

namespace Cg {

typedef std::vector<CGprogram> ProgramList;
typedef std::vector<CGparameter> ParameterList;

class Shader : Base::Shader
{
    CGcontext context;
    ProgramList vertex_programs;
    ProgramList fragment_programs;
public:
    ~Shader();

    void init();
    void clear();
    void load_program(ShaderType shader_type, std::string const &filename);
    bool bind_program(ShaderType shader_type, size_t index = 0);
    void unbind();

    CGparameter get_parameter(ShaderType shader_type, std::string const &name);
    std::vector<CGparameter> get_parameters(std::string const &name);
private:
    static void handle_error();
    void create_context();
    void choose_profiles();
};

}

}

#endif
