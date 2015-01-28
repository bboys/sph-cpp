#ifndef SPH_GRAPHICS_BASE_SHADER_H
#define SPH_GRAPHICS_BASE_SHADER_H

#include <iosfwd>
#include <vector>

namespace Graphics {

enum class ShaderType
{
    VERTEX,
    FRAGMENT,
    GEOMETRY
};

namespace Base {

class Shader
{
public:
    virtual void init() = 0;
    virtual void clear() = 0;
    virtual void load_program(ShaderType shader_type, std::string const &filename) = 0;
    virtual bool bind_program(ShaderType shader_type, size_t index = 0) = 0;
    virtual void unbind() = 0;
};

}

}

#endif
