#include "header_philo.h"

int onephilo(t_data *data)
{
    data->s_time = time_state();
    if(pthread_create(&*data->thread_id, NULL, &routine, &data->philos[0]))
        return(-1);
    pthread_detach(data->thread_id[0]);
    while(data->dead == 0)
        usleep(1);
    
    return (0);
}

int morephilo(t_data *data)
{
    int i;

    i = 0;
	data->s_time = time_state();
	while (i<data->phi_num)
	{
		if(pthread_create(&data->thread_id[i], NULL, &routine, &data->philos[i]))
            return -1;
		i++;
        nap(1);
	}
	i = 0;

	while (i < data->phi_num)
	{
		if(pthread_join(data->thread_id[i], NULL))
            return -1;
		i++;
	}
    return 0;
}
void *is_alive(void *phill)
{
	t_philos *entitie;

	entitie = (t_philos *) phill;
	while (entitie->data->dead == 0)
	{	
		pthread_mutex_lock(&entitie->block);	
		if(entitie->eated == entitie->data->n_toeat)
		{
			entitie->filled = 1;
			pthread_mutex_lock(&entitie->data->block);
			entitie->data->done++;
			entitie->eated++;
			pthread_mutex_unlock(&entitie->data->block);
		}		
		if(entitie->data->done >= entitie->data->phi_num)
			entitie->data->dead =1;
		if(entitie->eat_status == 0 && time_state() >= (unsigned) entitie->ttd)
			info_user(0, phill);
		pthread_mutex_unlock(&entitie->block);
	}
	return ((void *)0);
}

void *routine(void *phill)
{
    t_philos *entitie;

	pthread_t t1;
	entitie = (t_philos *) phill;
	entitie->ttd = entitie->data->ttd + time_state();
	if(pthread_create(&t1, NULL, &is_alive, (void *)entitie))
		return ((void *)1);
	while(entitie->data->dead == 0)
	{
		if(entitie->filled == 0 && entitie->data->dead == 0)
		{
			eat(phill);
			info_user(3, phill);
		}
	}
	if(pthread_join(t1, NULL))
		return (void *)1;
	return ((void *)0);
}
void destroy_frees(t_data *data)
{
	int i = 0;

	pthread_mutex_destroy(&data->to_print);
	pthread_mutex_destroy(&data->block);
	while(i < data->phi_num)
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
	exit(EXIT_SUCCESS);
}
