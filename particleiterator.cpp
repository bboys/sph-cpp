#include "particle.h"
#include "bucket.h"

namespace Physics {

Particle::neighbour_iterator::neighbour_iterator(Particle const *particle)
:
    bucket_iter(particle->d_bucket->neighbours.begin()),
    bucket_end(particle->d_bucket->neighbours.end()),
    particle_iter((*bucket_iter)->particles.begin()),
    particle_end((*bucket_iter)->particles.end())
{
    // Make sure particle_iter points to an actual particle
    while (particle_iter == particle_end and bucket_iter != bucket_end)
        operator++();
}

Particle::neighbour_iterator::neighbour_iterator(Particle const *particle, bool isend)
:
    bucket_iter(--particle->d_bucket->neighbours.end()),
    bucket_end(particle->d_bucket->neighbours.end()),
    particle_iter((*bucket_iter)->particles.end()),
    particle_end((*bucket_iter++)->particles.end())
{}

Particle::neighbour_iterator::neighbour_iterator(Particle::neighbour_iterator const &other)
:
    bucket_iter(other.bucket_iter),
    bucket_end(other.bucket_end),
    particle_iter(other.particle_iter),
    particle_end(other.particle_end)
{}

Particle::neighbour_iterator &Particle::neighbour_iterator::operator++()
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

    return *this;
}

Particle::neighbour_iterator const Particle::neighbour_iterator::operator++(int)
{
    neighbour_iterator tmp(*this); 
    operator++();
    return tmp;
}

bool Particle::neighbour_iterator::operator==(neighbour_iterator const &other) const
{
    return particle_iter == other.particle_iter and bucket_iter == other.bucket_iter;
}

bool Particle::neighbour_iterator::operator!=(neighbour_iterator const &other) const
{
    return particle_iter != other.particle_iter or bucket_iter != other.bucket_iter;
}

Particle *Particle::neighbour_iterator::operator*()
{
    return *particle_iter;
}

Particle **Particle::neighbour_iterator::operator->()
{
    return &(*particle_iter);
}

Particle::neighbour_iterator Particle::begin()
{
    return Particle::neighbour_iterator(this);
}

Particle::neighbour_iterator Particle::end()
{
    return Particle::neighbour_iterator(this, true);
}

}
