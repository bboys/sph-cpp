#ifndef SPH_PHYSICS_BUCKET_H
#define SPH_PHYSICS_BUCKET_H

#include <list>
#include <vector>
#include "particle.h"

namespace Physics {

class Bucket;

typedef std::vector<Bucket> BucketVec1;
typedef std::vector<BucketVec1> BucketVec2;
typedef std::vector<BucketVec2> BucketVec3;

class Bucket
{
public:
    ParticleVec particles;
    BucketVec neighbours;
public:
    void add_particle(Particle *particle);
    void remove_particle(Particle *particle);
    void add_neighbour(Bucket *other);
};

class Buckets
{
    BucketVec3 buckets;
    size_t x_size;
    size_t y_size;
    size_t z_size;

public:
    Buckets(size_t x, size_t y, size_t z);
    Bucket *get_bucket(float pos[3]);
private:
    void add_neighbours(Bucket &bucket, size_t i, size_t j, size_t k);
};

}

#endif
