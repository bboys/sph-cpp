#ifndef SPH_TIMER_H
#define SPH_TIMER_H

#include <sys/time.h>
#include <iostream>

class Timer
{
    double d_time;

    public:
        Timer();
        double tic();
        double toc();
        double tictoc();
};

Timer::Timer()
:
    d_time(0)
{}

inline double Timer::tic()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    d_time = (double)tv.tv_sec + 0.000001 * ((double)tv.tv_usec);
    return d_time;
}

inline double Timer::toc()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    double time = (double)tv.tv_sec + 0.000001 * ((double)tv.tv_usec);
    double elapsed_time = time - d_time;
    std::cout << "Time elapsed: " << elapsed_time << '\n';
    return elapsed_time;
}

inline double Timer::tictoc()
{
    double time = toc();
    tic();
    return time;
}
#endif
