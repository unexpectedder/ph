/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iazoubi <iazoubi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 06:34:17 by iazoubi           #+#    #+#             */
/*   Updated: 2024/10/31 14:22:52 by iazoubi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cclean(t_table *table)
{
	int		i;

	i = 0;
	if (!table)
		return ;

	if (table->forks)
	{
		while (i < table->ph_nbr)
		{
			pthread_mutex_destroy(&table->forks[i].fork);
		}
		pthread_mutex_destroy(&table->data_mutex);
		pthread_mutex_destroy(&table->print_mutex);
		pthread_mutex_destroy(&table->death_mutex);
		i++;
	}
	exit(1);
}

int main(int ac , char **av)
{
	t_table	table;
	t_philo	*philo;


	if (ac == 5 || ac == 6)
	{
		if (parsing(&table, av))
			return(cclean(&table), 1);
		if (init_table(&table))
			return (cclean(&table), 1);
		if (init_forks(&table))
			return(cclean(&table), 1);
		if (init_philos(&table))
			return (cclean(&table), 1);
		if (start_simulation(&table))
			return (cclean(&table), 1);
	}
	else
		g_error("error: not a valid nb of arguments");
	return (0);
}

