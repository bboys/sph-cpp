#ifndef SPH_GRAPHICS_BASE_OBJECT_H
#define SPH_GRAPHICS_BASE_OBJECT_H

#include <vector>
#include <cstring>

namespace Graphics {

namespace Base {

class Object {
public:
    float position[3];
    float color[4];

    Object() = default;
    Object(float const *p, float const *c)
    {
        memcpy(position, p, sizeof(float)*3);
        memcpy(color, c, sizeof(float)*4);
    };
};

template <class T>
class Objects : public std::vector<T>
{
public:
    Objects() = default;
    Objects(Objects const &other)
    :
        std::vector<T>(other)
    {};
};

}

}

#endif
