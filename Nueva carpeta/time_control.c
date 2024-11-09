#include "header_philo.h"

void	*eat(t_philos *philo)
{
	pthread_mutex_lock(philo->l_f);
	pthread_mutex_lock(philo->r_f);
	info_user(4, philo);
	info_user(5, philo);
	pthread_mutex_lock(&philo->block);
	philo->ttd = time_state() + philo->data->ttd;
	philo->eat_status = 1;
	philo->eated++;
	pthread_mutex_unlock(&philo->block);
	info_user(1, philo);
	nap(philo->data->tte);
	pthread_mutex_lock(&philo->block);
	philo->eat_status = 0;
	pthread_mutex_unlock(&philo->block);
	pthread_mutex_unlock(philo->l_f);
	pthread_mutex_unlock(philo->r_f);
	info_user(2, philo);
	nap(philo->data->tts);
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
	if (state == 0)
	{
		printf("\033[1;31m""At %u ms Philo %d is dead RIP\n", inst, phill->id);
		pthread_mutex_unlock(&phill->data->to_print);
		//exit(EXIT_FAILURE);
		destroy_frees(phill->data);
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
