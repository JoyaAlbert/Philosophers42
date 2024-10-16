#include "header_philo.h"

void    nap(unsigned int alarm)
{
    unsigned int gotobed;

    gotobed = time_state();
    while (((time_state() - gotobed)) < alarm)
        usleep(1);
}
unsigned int time_state(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000 + time.tv_usec/1000);
}
void info_user(int state)
{
    if(state == 0)
        printf("MUERTEEEE\n");
    if(state == 1)
        printf("COMEEEE\n");
    if (state == 2)
        printf("PIENSAAA\n");
    if (state == 3)
        printf("L FORK\n");
    if (state == 4)
        printf("R_FOORK\n");
}