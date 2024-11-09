#include "header_philo.h"

void	*eat(t_philos *philo)
{
	pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

    // Lock the left and right fork in order of memory address (or index)
    if (philo->l_f < philo->r_f) {
        first_fork = philo->l_f;
        second_fork = philo->r_f;
    } else {
        first_fork = philo->r_f;
        second_fork = philo->l_f;
    }
	pthread_mutex_lock(first_fork);
	pthread_mutex_lock(second_fork);
	info_user(4, philo);
	info_user(5, philo);
	pthread_mutex_lock(&philo->block);
	philo->ttd = time_state() + philo->data->ttd;
	philo->eated++;
	pthread_mutex_unlock(&philo->block);
	info_user(1, philo);
	nap(philo->data->tte);
	usleep(1);
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
	
	return (0);
}

void	nap(unsigned int alarm)
{
	unsigned int	gotobed;

	gotobed = time_state();
	while (((time_state() - gotobed)) < alarm)
		usleep(1);
}

unsigned int	time_state(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	info_user(int state, t_philos *phill)
{
	unsigned int	inst;

	pthread_mutex_lock(&phill->data->to_print);
	inst = time_state() - phill->data->s_time;
	if(phill->data->dead != 0)
	{
		pthread_mutex_unlock(&phill->data->to_print);
		return ((void)NULL);
	}
	if (state == 0)
	{
		printf("\033[1;31m""At %u ms Philo %d is dead RIP\n", inst, phill->id);
		phill->data->dead++;
	}
	if (state == 1)
		printf("At %u ms Philo %d is""\033[1;34m"
			" eating""\033[0m" "(taste bad)\n", inst, phill->id);
	if (state == 2)
		printf("At %u ms Philo %d is "
			"\033[1;32m""sleeping" "\033[0m""\n", inst, phill->id);
	if (state == 3)
		printf("At %u ms Philo %d is""\033[1;35m"
			" thinking\n""\033[0m", inst, phill->id);
	if (state == 4)
		printf("At %u ms Philo %d take left ""\033[1;33m"
			"fork \n""\033[0m", inst, phill->id);
	if (state == 5)
		printf("At %u ms Philo %d take right ""\033[1;33m"
			"fork \n""\033[0m", inst, phill->id);
	pthread_mutex_unlock(&phill->data->to_print);
}
