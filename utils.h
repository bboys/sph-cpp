#ifndef SPH_UTILS_H
#define SPH_UTILS_H

#include <iostream>
#include <cmath>

struct Difference
{
    float vec[3];
    float len2;
    float len;
};

inline float length2(float const vec[3])
{
    return vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
}

inline float length(float const vec[3])
{
    return sqrt(length2(vec));
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

template <class T>
inline T clamp(T x, T a, T b)
{
    return x < a ? a : (x > b ? b : x);
}

inline void set_zero(float vec1[3])
{
    vec1[0] = 0.0f;
    vec1[1] = 0.0f;
    vec1[2] = 0.0f;
}

inline float frand(float low, float high)
{
    static const float r_max = static_cast<float>(RAND_MAX);
    return low + static_cast<float>(rand())/r_max * (high - low);
}

inline int randint(int low, int high)
{
    return low + rand() % (high - low + 1);
}

template <class T>
inline void dump(T const &a)
{
    std::cout << a << std::endl;
}

inline void dump_vec(float const vec1[3])
{
    std::cout << vec1[0] << ' ' << vec1[1] << ' ' << vec1[2] << std::endl;
}

#endif
