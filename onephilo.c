#include "header_philo.h"

int onephilo(t_data *data)
{
    data->s_time = time_state();
    if(pthread_create(&*data->thread_id, NULL, &routine, &data->philos[0]))
        return(-1);
    pthread_detach(data->thread_id[0]);
    while(data->philos->dead == 0)
        usleep(1);
    
    return (0);
}