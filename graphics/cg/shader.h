#ifndef SHADER_H_ICS
#define SHADER_H_ICS

#define GL_GLEXT_PROTOTYPES

#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <string>
#include <iostream>
#include <vector>

typedef std::vector<CGprogram> ProgramList;
typedef std::vector<CGparameter> ParameterList;

enum class ShaderType
{
    VERTEX,
    FRAGMENT
};

class Shader
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

#endif
