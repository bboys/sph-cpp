#include "bucket.h"
#include <algorithm>
#include <cassert>
#include "utils.h"

namespace Physics {

void Bucket::remove_particle(Particle *particle)
{
    particles.remove(particle);
}

void Bucket::add_particle(Particle *particle)
{
    particles.push_back(particle);
}

void Bucket::add_neighbour(Bucket* other)
{
    neighbours.push_back(other);
}

Buckets::Buckets(size_t x, size_t y, size_t z)
:
    buckets(),
    x_size(x),
    y_size(y),
    z_size(z)
{
    for (size_t i = 0; i < x + 2; ++i)
    {
        buckets.push_back(BucketVec2());
        for (size_t j = 0; j < y + 2; ++j)
        {
            buckets.back().push_back(BucketVec1());
            for (size_t k = 0; k < z + 2; ++k)
                buckets.back().back().push_back(Bucket());
        }
    }

    size_t i3 = 0;
    for (BucketVec3::iterator v3 = buckets.begin(); v3 != buckets.end(); ++v3)
    {
        size_t i2 = 0;
        for (BucketVec2::iterator v2 = v3->begin(); v2 != v3->end(); ++v2)
        {
            size_t i1 = 0;
            for (BucketVec1::iterator v1 = v2->begin(); v1 != v2->end(); ++v1)
            {
                add_neighbours(*v1, i3, i2, i1);
                ++i1;
            }
            ++i2;
        }
        ++i3;
    }
}

void Buckets::add_neighbours(Bucket &bucket, size_t i3, size_t i2, size_t i1)
{
    if (i1 == 0 or i2 == 0 or i3 == 0)
        return;

    if (i3 > x_size or i2 > y_size or i1 > z_size)
        return;

    for (int i = -1; i < 2; ++i)
        for (int j = -1; j < 2; ++j)
            for (int k = -1; k < 2; ++k)
                bucket.add_neighbour(&buckets[i3+i][i2+j][i1+k]);
}

Bucket *Buckets::get_bucket(float pos[3])
{
    static const float min_dim = -1.0;
    static const float max_dim = 1.0;

    int i = (x_size - 1) / (max_dim - min_dim) * (pos[0] - min_dim) + 1;
    int j = (y_size - 1) / (max_dim - min_dim) * (pos[1] - min_dim) + 1;
    int k = (z_size - 1) / (max_dim - min_dim) * (pos[2] - min_dim) + 1;

    i = clamp(i, 0, static_cast<int>(x_size + 1));
    j = clamp(j, 0, static_cast<int>(y_size + 1));
    k = clamp(k, 0, static_cast<int>(z_size + 1));

    return &buckets[i][j][k];
}

}
