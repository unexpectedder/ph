/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iazoubi <iazoubi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:03:15 by iazoubi           #+#    #+#             */
/*   Updated: 2024/11/01 04:33:01 by iazoubi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->table->death_mutex);
	if (philo->table->end_sim)
	{
		pthread_mutex_unlock(&philo->table->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->death_mutex);
	pthread_mutex_lock(&philo->table->data_mutex);
	pthread_mutex_lock(&philo->table->print_mutex);
	printf("%ld %d %s\n", get_timestamp() - philo->table->start_sim \
		, philo->id, status);
	if (strncmp(status, "died", 4) != 0)
	{
		pthread_mutex_unlock(&philo->table->print_mutex);
	}
	pthread_mutex_unlock(&philo->table->data_mutex);
}

long	get_timestamp(void)
{
	struct timeval	time;
	long			store;

	gettimeofday(&time, NULL);
	store = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (store);
}

void	is_sleep(long wait_time)
{
	long	starting_time;
	long	time_passed;

	starting_time = get_timestamp();
	time_passed = get_timestamp() + wait_time;
	while (time_passed > starting_time)
	{
		usleep(100);
		starting_time = get_timestamp();
	}
}

void	simulation_end(t_table *table)
{
	pthread_mutex_lock(&table->death_mutex);
	table->end_sim = true;
	pthread_mutex_unlock(&table->death_mutex);
}
