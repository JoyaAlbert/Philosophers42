#include "header_philo.h"

unsigned int time_state(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000 + time.tv_usec/1000);
}