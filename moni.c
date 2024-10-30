/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moni.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iazoubi <iazoubi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:25:41 by iazoubi           #+#    #+#             */
/*   Updated: 2024/10/30 09:07:27 by iazoubi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *dining_cycle(t_philo *philo, t_table *table, bool *should_continue)
{
    pthread_mutex_lock(&philo->l_fork->fork);
    print_status(philo, "has taken a fork");
    pthread_mutex_lock(&philo->r_fork->fork);
    print_status(philo, "has taken a fork");
    pthread_mutex_lock(&table->data_mutex);
    if (!table->end_sim)
    {
        print_status(philo, "is eating");
        philo->last_meal = get_timestamp();
        philo->meals_counter++;
        if (table->max_meals != -1 && philo->meals_counter >= table->max_meals)
            philo->full = true;
    }
    pthread_mutex_unlock(&table->data_mutex);
    is_sleep(table->time_eat);
    pthread_mutex_unlock(&philo->l_fork->fork);
    pthread_mutex_unlock(&philo->r_fork->fork);
    print_status(philo, "is sleeping");
    is_sleep(table->time_sleep);
    print_status(philo, "is thinking");
    pthread_mutex_lock(&table->data_mutex);
    *should_continue = !table->end_sim;
    pthread_mutex_unlock(&table->data_mutex);
    
    return NULL;
}

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_table *table = philo->table;
    bool should_continue;

    pthread_mutex_lock(&table->data_mutex);
    philo->last_meal = get_timestamp();
    should_continue = !table->end_sim;
    pthread_mutex_unlock(&table->data_mutex);
    if (table->ph_nbr == 1)
    {
        print_status(philo, "has taken a fork");
        is_sleep(table->time_die);
        return NULL;
	}
    if (philo->id % 2 != 0)
	{
        // print_status(philo, "is thinking");
        is_sleep(100);
	}
    while (should_continue)
    {
        pthread_mutex_lock(&table->data_mutex);
        should_continue = !table->end_sim;
        pthread_mutex_unlock(&table->data_mutex);
        if (!should_continue)
            break;
        dining_cycle(philo, table, &should_continue);
    }
    return NULL;
}

void print_status(t_philo *philo, char *status)
{
    if (!philo->table->end_sim) 
	{
    	pthread_mutex_lock(&philo->table->print_mutex);
    	    printf("%ld %d %s\n", get_timestamp() - philo->table->start_sim, 
    	           philo->id, status);
    	pthread_mutex_unlock(&philo->table->print_mutex);	
	}
}
int death_checker(t_table *table, int ph_index)
{
	table->death_flag = 0;
	pthread_mutex_lock(&table->death_mutex);
    pthread_mutex_lock(&table->data_mutex);
	table->death_flag = 0;
    if ((get_timestamp() - table->philos[ph_index].last_meal) > table->time_die)
    {
		if (!table->end_sim)
		{
        	print_status(&table->philos[ph_index], "died");
        	table->end_sim = true;
        	table->death_flag = 1;
		}
    }
    pthread_mutex_unlock(&table->data_mutex);
	pthread_mutex_unlock(&table->death_mutex);
    return (table->death_flag);
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
            pthread_mutex_lock(&table->data_mutex);
            table->end_sim = true;
            pthread_mutex_unlock(&table->data_mutex);
            return NULL;
        }
        usleep(500);
    }
    return (NULL);
}

int start_simulation(t_table *table)
{
    int i;
    pthread_t monitor;

    i = 0;
    while (i < table->ph_nbr)
    {
        table->philos[i].table = table;
        if (pthread_create(&table->philos[i].tid, NULL, 
                          routine, &table->philos[i]) != 0)
            return (-1);
        i++;
    }
    if (pthread_create(&monitor, NULL, monitor_routine, table) != 0)
        return (-1);
	int index = -1;
	while(++index < table->ph_nbr)
		pthread_join(table->philos[index].tid, NULL);
		
    if (pthread_join(monitor, NULL) != 0)
        return (-1);
    return (0);
}

