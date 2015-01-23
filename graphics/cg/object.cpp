#include "object.h"
#include <cstring>

Object::Object(float const *p, float const *c)
{
    memcpy(position, p, sizeof(float)*3);
    memcpy(color, c, sizeof(float)*4);
}
