#include "header_philo.h"

void eat(t_philos *philo)
{
    pthread_mutex_lock(philo->l_f);
    pthread_mutex_lock(philo->r_f);
    printf("coge tenedor");
    pthread_mutex_lock(&philo->block);
    philo->ttd = time_state() + philo->data->ttd;
    philo->eat_status = 1;
    philo->eated++;
    nap(philo->data->tte);
    philo->eat_status = 0;
    printf("COME GUARRAAA\n");
    pthread_mutex_unlock(&philo->block);
    pthread_mutex_unlock(philo->l_f);
    pthread_mutex_unlock(philo->r_f);
}