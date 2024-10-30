/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iazoubi <iazoubi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 06:34:28 by iazoubi           #+#    #+#             */
/*   Updated: 2024/10/30 09:03:10 by iazoubi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
// #include <errno.h>  

typedef struct s_fork
{
    pthread_mutex_t    fork;
    int               fork_id;
}   t_fork;

typedef struct s_table
{
    long            ph_nbr;
    long            time_die;
    long            time_eat;
    long            time_sleep;
    long            max_meals;
    long            start_sim;
    _Atomic bool            end_sim;
	int				death_flag;
    pthread_mutex_t data_mutex;    // Protect shared data
    pthread_mutex_t print_mutex;   // Print synchronization
	pthread_mutex_t	death_mutex;
    t_fork          *forks;
    struct s_philo  *philos;        // Array of philosophers
}   t_table;

typedef struct s_philo
{
    pthread_t    tid;
    int         id;
    long        meals_counter;
    bool        full;
    long        last_meal;
    t_fork      *l_fork;
    t_fork      *r_fork;
    t_table     *table;
}   t_philo;


void 	ft_putstr_fd(char *str, int fd);
void		g_error(const char *err);
int		parsing(t_table *table, char **argv);
long	ft_atoi(const char *str);


int		init_table(t_table *table);
int		init_philos(t_table *table);
long	get_timestamp();
int		init_forks(t_table *table);
void 	print_status(t_philo *philo, char *status);
void 	*routine(void *arg);
int 	start_simulation(t_table *table);
void 	*moni(void *arg);
void	is_sleep(long wait_time);
void 	*dining_cycle(t_philo *philo, t_table *table, bool *should_continue);
int 	death_checker(t_table *table, int ph_index);


#endif