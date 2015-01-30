#ifndef SPH_GRAPHICS_CG_OBJECT_H
#define SPH_GRAPHICS_CG_OBJECT_H

#include <map>
#include <string>
#include <memory>
#include "shader.h"

namespace Graphics {

namespace Cg {

typedef std::map<std::string, ParameterList > ParameterMap;
typedef std::pair<std::string, ParameterList > ParameterPair;

class Objects
{
    std::shared_ptr<Shader> shader_ptr;
public:
    Shader &shader;
    ParameterMap parameters;

    Objects(): shader_ptr(new Shader()), shader(*shader_ptr), parameters() {};
    Objects(Objects const &other)
    :
        shader_ptr(other.shader_ptr),
        shader(*shader_ptr),
        parameters(other.parameters)
    {};

    void add_parameter(std::string const &name)
    {
        parameters.insert(ParameterPair(name, shader.get_parameters(name)));
    }

    virtual void init_shader()
    {
        parameters.clear();
        add_parameter("ModelViewProj");
        add_parameter("ModelViewI");
        add_parameter("ModelView");
        add_parameter("ModelViewIT");
        add_parameter("ProjI");
        add_parameter("wsize");
        add_parameter("lightpos");
        add_parameter("unproj_offset");
        add_parameter("unproj_scale");
        add_parameter("zb_offset");
        add_parameter("zb_scale");
        add_parameter("near");
        add_parameter("top");
        add_parameter("bottom");
        add_parameter("epsilon");
    }

};

}

}

#endif
