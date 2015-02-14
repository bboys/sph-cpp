#include "surface.h"
#include <cmath>

namespace Graphics {

namespace Base {

// Symmetric decaying spline with finite support, which is not specified.
// I took the Irwin-Hall distribution
float Surface::spline(float x) const
{
    x = 2.0 * x;
    if (x < -2.0)
        return 0.0;
    if (x < -1.0)
        return pow(2.0 + x, 3.0) / 4.0;
    if (x < 1.0)
        return (3.0 * pow(abs(x), 3.0) - 6.0 * pow(x, 2.0) + 4.0) / 4.0;
    if (x < 2.0)
        return pow(2.0 - x, 3.0) / 4.0;
    return 0.0;
}

// Isotropic smoothing kernel with dimension 3
float Surface::isotropic_kernel(Physics::Particle const &particle, Difference const &diff) const
{
    static const float sigma = 1.0; //Scaling factor, not a clue...
    return sigma / pow(particle.effective_radius, 3.0) * spline(diff.len / particle.effective_radius);
}

// Scalar field phi
float Surface::scalar_field(Physics::Buckets *buckets, float *position) const
{
    float phi = 0.0;
    Physics::Bucket *bucket = buckets->get_bucket(position);
    for (Physics::Bucket::neighbour_iterator it = bucket->begin(); it; ++it)
    {
        Physics::Particle *particle_i = *it;

        Difference diff;
        difference(position, particle_i->position, diff);

        phi += particle_i->mass / particle_i->density * isotropic_kernel(*particle_i, diff);
    }
    return phi;
}

}

}
