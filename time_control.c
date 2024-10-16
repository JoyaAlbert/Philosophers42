#include "header_philo.h"

void *eat(t_philos *philo)
{
    if(philo->data->dead != 0)
        return ((void *) 0);
    pthread_mutex_lock(philo->l_f);
    info_user(4, philo);
    pthread_mutex_lock(philo->r_f);
    info_user(5, philo);
    pthread_mutex_lock(&philo->block);
    philo->ttd = time_state() + philo->data->ttd;
    philo->eat_status = 1;
    philo->eated++;
    info_user(1, philo);
    nap(philo->data->tte);
    philo->eat_status = 0;
    pthread_mutex_unlock(&philo->block);
    pthread_mutex_unlock(philo->l_f);
    pthread_mutex_unlock(philo->r_f);
    info_user(2, philo);
    nap(philo->data->tts);
    return (0);
}

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
void info_user(int state, t_philos *phill)
{
    unsigned int inst;

    pthread_mutex_lock(&phill->data->to_print);
    inst = time_state() - phill->data->s_time;
    if(state == 0)
    {
        printf("At %u ms Philo %d has died RIP\n", inst, phill->id);
        destroy_frees(phill->data);
    }
    if(state == 1)
        printf("At %u ms Philo %d is eating (taste bad)\n", inst, phill->id);
    if (state == 2)
        printf("At %u ms Philo %d is sleeping\n", inst, phill->id);
    if (state == 3)
        printf("At %u ms Philo %d is thinking\n", inst, phill->id);
    if (state == 4)
        printf("At %u ms Philo %d take left fork \n", inst, phill->id);
    if (state == 5)
        printf("At %u ms Philo %d take right fork \n", inst, phill->id);
    pthread_mutex_unlock(&phill->data->to_print);
}
