#include "header_philo.h"

int	onephilo(t_data *data)
{
	data->s_time = time_state();
	if (pthread_create(&*data->thread_id, NULL, &life, &data->philos[0]))
		return (-1);
	while (data->dead == 0)
		usleep(1);
	return (0);
}


int	check_finished(t_philos *phill)
{
	t_data	*data;

	data = (t_data *) phill->data;
	//if (phill->eated == data->n_toeat)
	//{
	//	pthread_mutex_lock(&data->block);
	//	data->done++;
	//	pthread_mutex_unlock(&data->block);
	//}
	pthread_mutex_lock(&data->block);	
	if (data->done >= data->phi_num)
			return (0);
	if (data->done != data->phi_num && data->dead == 0)
	{
		pthread_mutex_unlock(&data->block);
		return (1);
	}
	pthread_mutex_unlock(&data->block);
	return (0);
}

void	*is_alive(void *p_data)
{
	int			i;
	unsigned int		time;
	t_data		*data;
	t_philos		*philo;

	data = (t_data *)p_data;
	philo = &(data->philos)[0];
	while (check_finished(philo) == 1)
	{
		i = 0;
		while (i < data->phi_num)
		{
			philo = &(data->philos)[i];
			pthread_mutex_lock(&philo->block);
			time = (unsigned) philo->ttd;
			pthread_mutex_unlock(&philo->block);
			if (time_state() > time)
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
	pthread_mutex_lock(&entitie->block);
	pthread_mutex_lock(&entitie->data->block);
	entitie->ttd = entitie->data->ttd + time_state();
	pthread_mutex_lock(&entitie->data->block);
	pthread_mutex_unlock(&entitie->block);
	if (entitie->id % 2 == 0)
		usleep(30);
	while (check_finished(phill) == 1)
	{
		eat(phill);
		info_user(3, phill);
	}
	return ((void *)0);
}
int	morephilo(t_data *data)
{
	int	i;
	pthread_t	status_thread;

	i = 0;
	data->s_time = time_state();
	if (pthread_create(&status_thread, NULL, &is_alive, (void *)data))
		return (1);
	pthread_detach(status_thread);
	while (i < data->phi_num)
	{
		if (pthread_create(&data->thread_id[i], NULL, &life, &data->philos[i]))
			return (-1);
		i++;
		usleep(0);
	}
	i = 0;
	while (i < data->phi_num)
	{
		if (pthread_join(data->thread_id[i], NULL))
			return (-1);
		usleep(0);
		i++;
	}
	if (pthread_join(status_thread, NULL))
		return (1);
	return (0);
}
void	destroy_frees(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&data->to_print);
	pthread_mutex_destroy(&data->block);
	while (i < data->phi_num)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos->l_f[i]);
		pthread_mutex_destroy(&data->philos->r_f[i]);
		pthread_mutex_destroy(&data->philos[i].block);
		i++;
	}
	if (data->thread_id != NULL)
		free(data->thread_id);
	if (data->philos != NULL)
		free(data->philos);
	if (data->forks != NULL)
		free(data->forks);
	free(data);
	exit(EXIT_SUCCESS);
}
