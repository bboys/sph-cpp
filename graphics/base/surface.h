#ifndef SPH_GRAPHICS_BASE_SURFACE_H
#define SPH_GRAPHICS_BASE_SURFACE_H

#include "../../particle.h"
#include "../../bucket.h"
#include "../../utils.h"
#include <memory>
#include "object.h"

namespace Graphics {

namespace Base {

// This Surface class aims to implement the surface reconstruction from
//"J. Yu & G. Turk, Reconstructing Surfaces of Particle-Based Fluids
// Using Anisotropic Kernels (2010)"
// http://www.cs.nyu.edu/~jihun/paper/sca2010.pdf

class Surface
{
    float spline(float x) const;
    float isotropic_kernel(Physics::Particle const &particle, Difference const &diff) const;
    float scalar_field(Physics::Buckets *buckets, float *position) const;
};

}

}

#endif
