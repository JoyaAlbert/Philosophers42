#include "header_philo.h"

int	check_finished(t_philos *phill)
{
	t_data	*data;

	pthread_mutex_lock(&phill->block);
	data = (t_data *) phill->data;
	pthread_mutex_unlock(&phill->block);
	pthread_mutex_lock(&phill->block);
	if (phill->eated == data->n_toeat)
	{
		pthread_mutex_unlock(&phill->block);
		return (0);
	}
	pthread_mutex_unlock(&phill->block);
	pthread_mutex_lock(&data->block);
	if (data->dead != 0)
	{
		pthread_mutex_unlock(&data->block);
		return (0);
	}
	pthread_mutex_unlock(&data->block);
	return (1);
}

void	*is_alive(void *p_data)
{
	int				i;
	unsigned int	time;
	t_data			*data;
	t_philos		*philo;
	int				n;

	data = (t_data *)p_data;
	philo = &(data->philos)[0];
	n = data->phi_num;
	while (check_finished(philo) == 1)
	{
		i = -1;
		while (++i < n)
		{
			philo = &(data->philos)[i];
			pthread_mutex_lock(&philo->block);
			time = (unsigned) philo->ttd;
			pthread_mutex_unlock(&philo->block);
			if (time_state() > time && data->dead != 1)
				info_user(0, philo);
		}
	}
	return ((void *)0);
}

void	*life(void *phill)
{
	t_philos	*entitie;

	entitie = (t_philos *) phill;
	if (entitie->id % 2 == 0)
		usleep(50);
	while (check_finished(entitie) == 1)
	{
		if (eat(entitie) == 0)
			break ;
		if (info_user(2, entitie) == 0)
			break ;
		nap(entitie->data->tts);
		if (info_user(3, entitie) == 0)
			break ;
		usleep(1);
	}
	return ((void *)0);
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
