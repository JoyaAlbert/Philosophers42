#include "header_philo.h"

void	startimes(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->phi_num)
		data->philos[i].ttd = data->s_time + (unsigned int) data->ttd;
}

int	onephilo(t_data *data)
{
	data->s_time = time_state();
	info_user(4, data->philos);
	nap(data->ttd);
	info_user(0, data->philos);
	return (0);
}

int	morephilo(t_data *data)
{
	int			i;
	pthread_t	status_thread;

	i = -1;
	data->s_time = time_state();
	startimes(data);
	if (pthread_create(&status_thread, NULL, &is_alive, (void *)data))
		return (-1);
	pthread_detach(status_thread);
	while (++i < data->phi_num)
	{

		if (pthread_create(&data->thread_id[i], NULL, &life, &data->philos[i]))
			return (-1);
		usleep(1);
	}
	i = -1;
	while (++i < data->phi_num)
	{
		if (pthread_join(data->thread_id[i], NULL))
			return (-1);
		usleep(1);
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
	free(data->thread_id);
	free(data->philos);
	free(data->forks);
	free(data);
}
