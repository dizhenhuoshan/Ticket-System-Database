#ifndef MAIN_CPP_TIMER_H
#define MAIN_CPP_TIMER_H

#include <sys/time.h>
#include <iostream>
#include <time.h>

class timer {
public:
    timer() = default;
    ~timer() = default;

    double get_time() {
        struct timeval time ;
        if (gettimeofday(&time, nullptr)){
            return 0;
        }
        return (double)time.tv_sec + (double)time.tv_usec * .000001;
    }
};


#endif //MAIN_CPP_TIMER_H
