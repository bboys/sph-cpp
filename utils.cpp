#include "utils.h"
#include <cmath>
#include <cstdlib> 

inline float length2(float const vec[3])
{
    return vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
}

inline float length(float const vec[3])
{
    return sqrt(length2(vec));
}

inline float dot(float const vec1[3], float const vec2[3])
{
    return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}

inline void difference(float const vec1[3], float const vec2[3], float out[3])
{
    out[0] = vec1[0] - vec2[0];
    out[1] = vec1[1] - vec2[1];
    out[2] = vec1[2] - vec2[2];
}

inline void difference(float const vec1[3], float const vec2[3], Difference &out)
{
    out.vec[0] = vec1[0] - vec2[0];
    out.vec[1] = vec1[1] - vec2[1];
    out.vec[2] = vec1[2] - vec2[2];
    out.len2 = length2(out.vec);
    out.len = sqrt(out.len2);
}

inline void set_zero(float vec1[3])
{
    for (size_t idx = 0; idx != 3; ++idx)
        vec1[idx] = 0.0f;
}

inline void dump_vec(float const vec1[3])
{
    std::cout << vec1[0] << ' ' << vec1[1] << ' ' << vec1[2] << std::endl;
}

inline float frand(float low, float high)
{
    static const float r_max = static_cast<float>(RAND_MAX);
    return low + static_cast<float>(rand())/r_max * (high - low);
}
