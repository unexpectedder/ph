/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moni.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iazoubi <iazoubi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:25:41 by iazoubi           #+#    #+#             */
/*   Updated: 2024/10/31 14:56:27 by iazoubi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *dining_cycle(t_philo *philo, t_table *table)
{   
	pthread_mutex_lock(&philo->l_fork->fork);
    print_status(philo, "has taken a fork1");
	if (table->ph_nbr == 1)
    {
        is_sleep(table->time_die);
        return NULL;
	}
    pthread_mutex_lock(&philo->r_fork->fork);
    print_status(philo, "has taken a fork2");

    pthread_mutex_lock(&table->data_mutex);
    philo->last_meal = get_timestamp();
    philo->meals_counter++;
    if (table->max_meals != -1 && philo->meals_counter >= table->max_meals)
        philo->full = true;
    pthread_mutex_unlock(&table->data_mutex);

    print_status(philo, "is eating");
    is_sleep(table->time_eat);
    pthread_mutex_unlock(&philo->l_fork->fork);
    pthread_mutex_unlock(&philo->r_fork->fork);
    print_status(philo, "is sleeping");
    is_sleep(table->time_sleep);
    print_status(philo, "is thinking");
    
    return NULL;
}

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_table *table = philo->table;
	
	// table->start_sim = get_timestamp();
    if (philo->id % 2 == 0)
	{
		// usleep(500);
        print_status(philo, "is thinking");
        is_sleep(30);
	}
    while (1)
    {
        pthread_mutex_lock(&table->death_mutex);
        if (table->end_sim) {	
        	pthread_mutex_unlock(&table->death_mutex);
            break;
		}
        pthread_mutex_unlock(&table->death_mutex);
        dining_cycle(philo, table);
		if (table->ph_nbr == 1)
			break;
    }
    return NULL;
}

void print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->table->death_mutex);
	if (philo->table->end_sim){
		pthread_mutex_unlock(&philo->table->death_mutex);
		return;
	}
	pthread_mutex_unlock(&philo->table->death_mutex);
	
	pthread_mutex_lock(&philo->table->data_mutex);

	pthread_mutex_lock(&philo->table->print_mutex);
	printf("%ldms %d %s\n", get_timestamp() - philo->table->start_sim \
		, philo->id, status);
	pthread_mutex_unlock(&philo->table->print_mutex);

	pthread_mutex_unlock(&philo->table->data_mutex);
}
int death_checker(t_table *table, int ph_index)
{
	pthread_mutex_lock(&table->death_mutex);
	if (table->end_sim)
		return (true);
	pthread_mutex_unlock(&table->death_mutex);
	
	pthread_mutex_lock(&table->data_mutex);
	if ((get_timestamp() - table->philos[ph_index].last_meal) > table->time_die) 
	{
		    pthread_mutex_unlock(&table->data_mutex);	
        	print_status(&table->philos[ph_index], "died");
			
			pthread_mutex_lock(&table->death_mutex);
        	table->end_sim = true;
			pthread_mutex_unlock(&table->death_mutex);
			return (true);
	}
    pthread_mutex_unlock(&table->data_mutex);
	return (false);
}

void *monitor_routine(void *arg)
{
    t_table *table = (t_table *)arg;
    int     i;
    int  full_count;
	
    while (1)
    {
        i = 0;
        full_count = 0;
        while (i < table->ph_nbr)
        {
			if (death_checker(table, i))
				return (NULL);
			pthread_mutex_lock(&table->data_mutex);
            if (table->philos[i].full)
                full_count++;
            pthread_mutex_unlock(&table->data_mutex);
            i++;
        }
        if (table->max_meals != -1 && full_count == table->ph_nbr)
        {
            pthread_mutex_lock(&table->death_mutex);
            table->end_sim = true;
            pthread_mutex_unlock(&table->death_mutex);
            return NULL;
        }
        usleep(500); // usleep
    }
    return (NULL);
}

int start_simulation(t_table *table)
{
    int i;
    pthread_t monitor;
	int index;
    i = 0;
	table->start_sim = get_timestamp();
    while (i < table->ph_nbr)
    {
        table->philos[i].table = table;
		table->philos[i].last_meal = table->start_sim;
	    if (pthread_create(&table->philos[i].tid, NULL, \
            routine, &table->philos[i]) != 0)
            return (-1);
        i++;
    }
    if (pthread_create(&monitor, NULL, monitor_routine, table) != 0)
        return (-1);
	index = -1;
	while(++index < table->ph_nbr)
		if (pthread_join(table->philos[index].tid, NULL) != 0)
			return (-1);
    if (pthread_join(monitor, NULL) != 0)
        return (-1);
    return (0);
}


