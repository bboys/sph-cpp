#include "bucket.h"

namespace Physics {

Bucket::neighbour_iterator::neighbour_iterator(Bucket *bucket)
:
    bucket_iter(bucket->neighbours.begin()),
    bucket_end(bucket->neighbours.end()),
    particle_iter((*bucket_iter)->particles.begin()),
    particle_end((*bucket_iter)->particles.end()),
    valid(true)
{
    // Make sure particle_iter points to an actual particle
    while (particle_iter == particle_end and bucket_iter != bucket_end)
        operator++();
}

Bucket::neighbour_iterator::neighbour_iterator(Bucket *bucket, bool isend)
:
    bucket_iter(--bucket->neighbours.end()),
    bucket_end(bucket->neighbours.end()),
    particle_iter((*bucket_iter)->particles.end()),
    particle_end((*bucket_iter++)->particles.end()),
    valid(false)
{}

Bucket::neighbour_iterator::neighbour_iterator(Bucket::neighbour_iterator const &other)
:
    bucket_iter(other.bucket_iter),
    bucket_end(other.bucket_end),
    particle_iter(other.particle_iter),
    particle_end(other.particle_end),
    valid(other.valid)
{}

Bucket::neighbour_iterator &Bucket::neighbour_iterator::operator++()
{
    if (++particle_iter != particle_end)
        return *this;

    while (++bucket_iter != bucket_end)
    {
        particle_iter = (*bucket_iter)->particles.begin();
        particle_end = (*bucket_iter)->particles.end();
        if (particle_iter != particle_end)
            return *this;
    }

    valid = false;

    return *this;
}

Bucket::neighbour_iterator const Bucket::neighbour_iterator::operator++(int)
{
    neighbour_iterator tmp(*this); 
    operator++();
    return tmp;
}

bool Bucket::neighbour_iterator::operator==(neighbour_iterator const &other) const
{
    return particle_iter == other.particle_iter;
}

bool Bucket::neighbour_iterator::operator!=(neighbour_iterator const &other) const
{
    return particle_iter != other.particle_iter;
}

Particle *Bucket::neighbour_iterator::operator*()
{
    return *particle_iter;
}

Particle **Bucket::neighbour_iterator::operator->()
{
    return &(*particle_iter);
}

Bucket::neighbour_iterator Bucket::begin()
{
    return Bucket::neighbour_iterator(this);
}

Bucket::neighbour_iterator Bucket::end()
{
    return Bucket::neighbour_iterator(this, true);
}

}
