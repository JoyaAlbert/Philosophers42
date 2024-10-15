#ifndef HEADER_PHILO_H
# define HEADER_PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_start_data
{
    unsigned int s_time;
    struct s_philos *philos;
    int phi_num;
    int ttd;
    int tte;
    int tts;
    int n_toeat;
    pthread_t *thread_id;
    pthread_mutex_t block;
    pthread_mutex_t open;
    pthread_mutex_t *forks;
} t_data;

typedef struct s_philos
{
    t_data *data;
    int id;
    int eated;
    pthread_mutex_t *r_f;
    pthread_mutex_t *l_f;;
} t_philos;

//threads
void *routine(void *phil);

//utils
long	ft_atoi(char *str);
int     ft_isdigit(int c);
//timing
unsigned int time_state(void);
#endif
