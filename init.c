/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iazoubi <iazoubi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:52:14 by iazoubi           #+#    #+#             */
/*   Updated: 2024/11/01 04:16:47 by iazoubi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_forks(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->ph_nbr)
	{
		table->forks[i].fork_id = i;
		if (pthread_mutex_init(&table->forks[i].fork, NULL) != 0)
		{
			return (-1);
		}
		i++;
	}
	return (0);
}

int	init_philos(t_table *table)
{
	int	i;

	i = 0;
	table->philos = malloc(sizeof(t_table) * table->ph_nbr);
	if (!table->philos)
		return (-1);
	while (i < table->ph_nbr)
	{
		table->philos[i].id = 1 + i;
		table->philos[i].meals_counter = 0;
		table->philos[i].full = false;
		if (i == (table->ph_nbr))
		{
			table->philos[i].l_fork = &table->forks[0];
			table->philos[i].r_fork = &table->forks[i];
		}
		else
		{
			table->philos[i].l_fork = &table->forks[i];
			table->philos[i].r_fork = &table->forks[i + 1];
		}
		i++;
	}
	return (0);
}

int	init_table(t_table *table)
{
	if (pthread_mutex_init(&table->data_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&table->data_mutex);
		return (-1);
	}
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&table->print_mutex);
		return (-1);
	}
	if (pthread_mutex_init(&table->death_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&table->print_mutex);
		pthread_mutex_destroy(&table->data_mutex);
		return (-1);
	}
	table->forks = malloc(sizeof(t_fork) * table->ph_nbr);
	if (!table->forks)
	{
		pthread_mutex_destroy(&table->print_mutex);
		pthread_mutex_destroy(&table->data_mutex);
		pthread_mutex_destroy(&table->death_mutex);
		return (-1);
	}
	return (0);
}
