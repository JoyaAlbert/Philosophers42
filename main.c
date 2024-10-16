#include "header_philo.h"

void fork_data_init(t_data *data)
{
	int i;

	i = 0;
	data->done = 0;
	data->dead = 0;
	pthread_mutex_init(&data->block, NULL);
	pthread_mutex_init(&data->to_print, NULL);
	while(i < data->phi_num)
	{
		data->philos[i].eat_status = 0;
		data->philos[i].filled = 0;
		data->philos[i].r_f = &data->forks[i];
		if(i == 0)
			data->philos[i].l_f = &data->forks[data->phi_num - 1];
		else
			data->philos[i].l_f = &data->forks[i - 1];
		i++;
	}
}

int datainit(int argc, t_data *data)
{
	int i;

	i = 0;
	if(data->phi_num <= 0 || data->ttd <= 0 || data->tte <=0 || data->tts <= 0
		|| data->phi_num > 200)
		return -1;
	if(argc != 6)
		data->n_toeat = -1;
	data->philos = malloc(sizeof(t_philos) * data->phi_num);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->phi_num);
	data->thread_id = malloc(sizeof(pthread_t) * data->phi_num);
	if(data->philos == NULL || data->forks == NULL || data->thread_id == NULL)
		return (-1);
	while(i < data->phi_num)
	{
		data->philos[i].data = data;
		data->philos[i].eated = 0;
		data->philos[i].id = i;
		data->philos[i].ttd = data->ttd;
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&data->philos[i].block, NULL);
		i++;
	}
	fork_data_init(data);
	return (0);
}
int datatake(char **argv, t_data *data)
{
	int i;
	int	j;

	i = 1;
	while (argv[i] != NULL)
	{
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (ft_isdigit(argv[i][j]) == 0)
				return -1;
			j++;
		}
		i++;
	}
	data->phi_num = ft_atoi(argv[1]);
	data->ttd = ft_atoi(argv[2]);
	data->tte = ft_atoi(argv[3]);
	data->tts = ft_atoi(argv[4]);
	if (argv[5] != NULL)
		data->n_toeat = ft_atoi(argv[5]);
	else
		data->n_toeat = -1;
	return (datainit(i, data));
}

int	main(int argc, char **argv)
{
	t_data data;
	
	if (argc < 5 || argc > 6)
		printf("Input example-> ./philo 5 800 200 200 7(optional)\n");
	else
	{
		if(datatake(argv, &data) == -1)
		{
			printf("Acepted Args->Only positive numbers|0 < nºphill < 200\n");
			printf("\n\nInput example-> ./philo 5 800 200 200 7(optional)\n\n");
			return (0);
		}
		if(data.phi_num == 1)
			onephilo(&data);
		else
			morephilo(&data);
	}
	destroy_frees(&data);
	return (0);
}
