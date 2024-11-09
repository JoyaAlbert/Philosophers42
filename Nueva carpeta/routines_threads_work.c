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
	t_data			*data;
	t_philos		*philo;
	int	n;

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
	if (entitie->id+1 % 2 == 0)
		usleep(50);
	while (check_finished(entitie) == 1)
	{
		eat(entitie);
		//if(entitie->data->dead != 0)
		//	break;
		usleep(1);
		//if(entitie->data->dead != 0)
		//	break;
		info_user(2, phill);
		nap(entitie->data->tts);
		usleep(1);
		info_user(3, phill);
	}
	return ((void *)0);
}

void startimes(t_data *data)
{
	int	i;

	i = -1;
	while(++i < data->phi_num)
		data->philos[i].ttd = data->s_time + (unsigned int) data->ttd;
}
int	morephilo(t_data *data)
{
	int	i;
	pthread_t	status_thread;

	i = 0;
	data->s_time = time_state();
	if (pthread_create(&status_thread, NULL, &is_alive, (void *)data))
		return (-1);
	pthread_detach(status_thread);
	while (i < data->phi_num)
	{
		if (pthread_create(&data->thread_id[i], NULL, &life, &data->philos[i]))
			return (-1);
		i++;
		usleep(1);
	}
	i = 0;
	while (i < data->phi_num)
	{
		if (pthread_join(data->thread_id[i], NULL))
			return (-1);
		usleep(1);
		i++;
	}
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
	//	if (data->thread_id != NULL)
		free(data->thread_id);
	//if (data->philos != NULL)
	free(data->philos);
	//if (data->forks != NULL)
		free(data->forks);
	free(data);
}
