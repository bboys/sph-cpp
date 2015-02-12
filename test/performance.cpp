#include "particle.h"
#include "bucket.h"
#include "utils.h"
#include <memory>
#include "timer.h"
#include <cstring>

int main(int argc, char *argv[])
{
    Timer timer;

    std::cout << "Filling with particles" << std::endl;
    timer.tic();
    int max_particles = 4000;
    size_t bucket_num = 20;
    float tstep = 0.01;
    std::unique_ptr<Physics::Buckets> buckets(new Physics::Buckets(bucket_num, bucket_num, bucket_num));
    Physics::ParticlesBase particles;

    Physics::Particle::init_wall();

    float pos[3] = {0.9f,0.9f,0.9f};
    float vel[3] = {-3.0f,0.0f,0.0f};
    for (int  i = 0; i != max_particles; ++i)
    {
        pos[0] += frand(-1e-6,1e-6);
        pos[1] += frand(-1e-6,1e-6);
        pos[2] += frand(-1e-6,1e-6);
        Physics::Particle particle(pos, buckets.get());
        particles.push_back(particle);
        memcpy(particles.back().velocity, vel, sizeof(float)*3);

        particles.update(tstep);
    }
    timer.toc();
}
