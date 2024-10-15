#include "header_philo.h"

void *is_alive(void *phill)
{
	t_philos *entitie;

	entitie = (t_philos *) phill;
	while (entitie->dead == 0)
	{
		if(entitie->data->done >= entitie->data->phi_num)
			entitie->dead =1;
		pthread_mutex_lock(&entitie->block);
		if(entitie->eated == entitie->data->n_toeat)
		{ //lo mismo hay q bloquear data con un mutex lock
			pthread_mutex_lock(&entitie->data->block);
			entitie->data->done++;
			entitie->eated++;
			pthread_mutex_unlock(&entitie->data->block);
		}
		if(entitie->eat_status == 0 && time_state() >= (unsigned) entitie->ttd)
		{
			printf("muereeeee");
			entitie->dead = 1;
		}
		pthread_mutex_unlock(&entitie->block);
	}
	return (0);
}

void *routine(void *phill)
{
    t_philos *entitie;

	entitie = (t_philos *) phill;
	entitie->ttd = entitie->data->ttd + time_state();
	if(pthread_create(&entitie->first_thread, NULL, &is_alive, (void *)entitie))
		return (void *)1;
	while(entitie->dead == 0)
	{
		eat(phill);
		printf("piensa putaaaa");
	}
	if(pthread_join(entitie->first_thread, NULL))
		return (void *)1;
	return ((void *)0);
}
void destroy_frees(t_data *data)
{
	int i = 0;

	while(i <data->phi_num)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].block);
		i++;
	}
	pthread_mutex_destroy(&data->block);
	if (data->thread_id != NULL)
		free(data->thread_id);
	if (data->philos != NULL)
		free(data->philos);
	if (data->forks != NULL)
		free(data->forks);
}