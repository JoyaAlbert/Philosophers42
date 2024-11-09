#include "header_philo.h"

int	check_finished(t_philos *phill)
{
	t_data	*data;

	pthread_mutex_lock(&phill->block);
	data = (t_data *) phill->data;
	pthread_mutex_unlock(&phill->block);
	pthread_mutex_lock(&data->philos->block);
	if (data->philos->eated == data->n_toeat)
	{
		pthread_mutex_lock(&data->block);
		data->done++;
		pthread_mutex_unlock(&data->block);
	}
	pthread_mutex_unlock(&data->philos->block);
	pthread_mutex_lock(&data->block);
	if (data->done == data->phi_num)
		data->dead++;
	if (data->done == data->phi_num || data->dead > 0)
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
		usleep(1);
	}
	return ((void *)0);
}

void	*life(void *phill)
{
	t_philos	*entitie;

	entitie = (t_philos *) phill;
	if (entitie->id + 1 % 2 == 0)
		usleep(50);
	while (check_finished(entitie) == 1)
	{
		eat(entitie);
		usleep(1);
		info_user(2, phill);
		nap(entitie->data->tts);
		usleep(1);
		info_user(3, phill);
	}
	return ((void *)0);
}
