#include "header_philo.h"

int onephilo(t_data *data)
{
    data->s_time = time_state();
    if(pthread_create(&*data->thread_id, NULL, &routine, &*data->philos));
        return(-1);
    return (0);
}