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

typedef std::list<Particle *> ParticleVec;
typedef std::list<Particle *>::iterator ParticleIter;

typedef std::list<Bucket *> BucketVec;
typedef std::list<Bucket *>::iterator BucketIter;

class Bucket
{
public:
    ParticleVec particles;
    BucketVec neighbours;
public:
    void add_particle(Particle *particle);
    void remove_particle(Particle *particle);
    void add_neighbour(Bucket *other);

    class neighbour_iterator: public std::iterator<std::input_iterator_tag, Particle *>
    {
        BucketIter bucket_iter;
        BucketIter bucket_end;
        ParticleIter particle_iter;
        ParticleIter particle_end;

        bool valid;

    public:
        neighbour_iterator() = delete;
        neighbour_iterator(Bucket *bucket);
        neighbour_iterator(Bucket *bucket, bool isend);
        neighbour_iterator(neighbour_iterator const &other);
        neighbour_iterator &operator++();
        neighbour_iterator const operator++(int);
        bool operator==(neighbour_iterator const &other) const;
        bool operator!=(neighbour_iterator const &other) const;
        Particle *operator*();
        Particle **operator->();

        // Method to increase performance. The iterator evaluates as false
        // when the end is reached.
        operator bool() const {return valid;}
    };

    neighbour_iterator begin();
    neighbour_iterator end();
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
