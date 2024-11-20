#include "header_philo.h"

void *left(t_philos *philo, int n)
{
	if (philo->id % 2 == 0 && n % 2 == 0)
	{
		pthread_mutex_unlock(philo->r_f);
		pthread_mutex_unlock(philo->l_f);
	}
	else
	{
		pthread_mutex_unlock(philo->l_f);
		pthread_mutex_unlock(philo->r_f);
	}	
	return ((void *)0);
}
void	*eat(t_philos *philo)
{
	int	n;

	pthread_mutex_lock(&philo->data->block);
	n = philo->data->phi_num;
	pthread_mutex_unlock(&philo->data->block);
	if (philo->id % 2 == 0  && n % 2 == 0)
	{
		pthread_mutex_lock(philo->l_f);
		pthread_mutex_lock(philo->r_f);
	}
	else
	{
		pthread_mutex_lock(philo->r_f);
		pthread_mutex_lock(philo->l_f);
	}
	if(info_user(4, philo)== 0)
		return (left(philo, n));
	if(info_user(5, philo)==0)
		return (left(philo, n));
	pthread_mutex_lock(&philo->block);
	philo->ttd = time_state() + philo->data->ttd;	
	philo->eated++;
	pthread_mutex_unlock(&philo->block);
	if(info_user(1, philo)==0)
		return (left(philo, n));
	nap(philo->data->tte);
	return (left(philo, n));
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

void	printmsg(int state, t_philos *phill, unsigned int inst)
{
	pthread_mutex_lock(&phill->data->to_print);
	pthread_mutex_lock(&phill->data->block);
	if (state == 0)
		phill->data->dead++;
	if (state == 1)
		printf("At %u ms Philo %d is""\033[1;34m"
			" eating\n", inst, phill->id + 1);
	if (state == 2)
		printf("At %u ms Philo %d is "
			"\033[1;32m""sleeping\n", inst, phill->id + 1);
	if (state == 3)
		printf("At %u ms Philo %d is""\033[1;35m"
			" thinking\n", inst, phill->id + 1);
	if (state == 4)
		printf("At %u ms Philo %d take 1st ""\033[1;33m"
			"fork \n", inst, phill->id + 1);
	if (state == 5)
		printf("At %u ms Philo %d take 2nd ""\033[1;33m"
			"fork \n", inst, phill->id + 1);
	if (state == 0)
		printf("\033[1;31m""At %u ms Philo %d is dead RIP\n",
			inst, phill->id + 1);
	printf("\033[0m");
	pthread_mutex_unlock(&phill->data->to_print);
	pthread_mutex_unlock(&phill->data->block);
}

int	info_user(int state, t_philos *phill)
{
	unsigned int	inst;

	pthread_mutex_lock(&phill->data->block);
	inst = time_state() - phill->data->s_time;
	if (phill->data->dead != 0)
	{
		pthread_mutex_unlock(&phill->data->block);
		return(0);
	}
	pthread_mutex_unlock(&phill->data->block);
	printmsg(state, phill, inst);
	return (1);
}
