#include "shader.h"
#include <cassert>
#include <iostream>

namespace Graphics {

namespace Cg {

CGprofile vertex_profile;
CGprofile fragment_profile;

bool silent_shader = true;

Shader::~Shader()
{
    clear();
}

void Shader::init()
{
    clear();

    cgSetErrorCallback(handle_error);
    create_context();
    choose_profiles();
}

void Shader::clear()
{
    vertex_programs.clear();
    fragment_programs.clear();
    if (context)
        cgDestroyContext(context);
}

void Shader::handle_error()
{
    std::cerr << "Cg error: " << cgGetErrorString(cgGetError()) << std::endl;
}

void Shader::create_context()
{
    context = cgCreateContext();
}

void Shader::choose_profiles()
{
    if (vertex_profile)
        return;

    vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
    cgGLSetOptimalOptions(vertex_profile);
    fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    cgGLSetOptimalOptions(fragment_profile);
    if (silent_shader) return;
    std::cout << "vertex profile:   " << cgGetProfileString(vertex_profile) << std::endl;
    std::cout << "fragment profile: " << cgGetProfileString(fragment_profile) << std::endl;
}

void Shader::load_program(ShaderType shader_type, std::string const &filename)
{
    CGprogram program;
    CGprofile profile;

    std::string path = "graphics/cg/";

    assert(cgIsContext(context));

    if (shader_type == ShaderType::VERTEX)
        profile = vertex_profile;
    else
        profile = fragment_profile;

    if (!silent_shader)
    std::cerr << "Cg program " << filename << " creating." << std::endl;
    program = cgCreateProgramFromFile(context, CG_SOURCE,
            (path+filename).c_str(), profile, NULL, NULL);

    if(!cgIsProgramCompiled(program)) {
        std::cout << cgGetLastListing(context) << std::endl;
        exit(1);
    }

    if (!silent_shader)
    std::cerr << "Cg program " << filename << " loading." << std::endl;
    cgGLLoadProgram(program);
    
    
    if (shader_type == ShaderType::VERTEX)
        vertex_programs.push_back(program);
    else
        fragment_programs.push_back(program);

    if (!silent_shader)
    std::cerr << "Cg program " << filename << " loaded." << std::endl;
}

bool Shader::bind_program(ShaderType shader_type, size_t index)
{
    cgGLSetManageTextureParameters(context, GL_TRUE);
    if (shader_type == ShaderType::VERTEX and vertex_programs.size() > index)
    {
        cgGLEnableProfile(vertex_profile);
        cgGLBindProgram(vertex_programs[index]);
        return true;
    }

    if (shader_type == ShaderType::FRAGMENT and fragment_programs.size() > index)
    {
        cgGLEnableProfile(fragment_profile);
        cgGLBindProgram(fragment_programs[index]);
        return true;
    }

    return false;
}

void Shader::unbind()
{
    cgGLDisableProfile(vertex_profile);
    cgGLDisableProfile(fragment_profile);
}

CGparameter Shader::get_parameter(ShaderType shader_type, std::string const &name)
{
    if (shader_type == ShaderType::VERTEX)
        return cgGetNamedParameter(vertex_programs[0], name.c_str());
    return cgGetNamedParameter(fragment_programs[0], name.c_str());
}

std::vector<CGparameter> Shader::get_parameters(std::string const &name)
{
    ParameterList parameters;
    for (ProgramList::iterator it = vertex_programs.begin(); it != vertex_programs.end(); ++it)
        if (CGparameter param = cgGetNamedParameter(*it, name.c_str()))
            parameters.push_back(param);
    for (ProgramList::iterator it = fragment_programs.begin(); it != fragment_programs.end(); ++it)
        if (CGparameter param = cgGetNamedParameter(*it, name.c_str()))
            parameters.push_back(param);
    return parameters;
}

}

}
