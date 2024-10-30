/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iazoubi <iazoubi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 06:35:54 by iazoubi           #+#    #+#             */
/*   Updated: 2024/10/30 09:35:00 by iazoubi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(const char *s)
{
	size_t		i;
	i = 0;
	while (s[i])
		i++;
	return (i);
}

void ft_putstr_fd(char *str, int fd)
{
    if (!str)
        return;
    while (*str)
    {
        write(fd, str, 1);
        str++;
    }
}
void g_error(const char *err)
{
	printf("%s\n", err);
	// return (0);
}
size_t	ft_strlcpy(char *dst, const char *src, size_t n)
{
	size_t	i;
	size_t	src_size;

	src_size = ft_strlen(src);
	i = 0;
	if (n > 0)
	{
		while (i < n - 1 && src[i] != '\0')
		{
			dst[i] = src[i];
			i++;
		}
		if (n <= src_size)
			dst[n - 1] = '\0';
		else if (n > 0)
			dst[i] = '\0';
	}
	return (src_size);
}
char	*ft_strchr(const char *str, int c)
{
	size_t	i;
	char	*str1;

	i = 0;
	str1 = (char *)str;
	while (str1[i] != '\0')
	{
		if (str1[i] == (char)c)
			return (&str1[i]);
		i++;
	}
	if (str1[i] == (char)c)
		return (&str1[i]);
	return (NULL);
}


char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	beg;
	size_t	end;
	char	*str;

	if (!s1 || !set)
		return (NULL);
	beg = 0;
	end = ft_strlen(s1);
	while (s1[beg] && ft_strchr(set, s1[beg]))
		beg++;
	if (s1[beg] != '\0')
		while (s1[end - 1] && ft_strchr(set, s1[end - 1]))
			end--;
	str = malloc((end - beg) + 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, &s1[beg], end - beg + 1);
	return (str);
}

long		ft_atoi(const char *str)
{
	char *s = ft_strtrim(str, " \t\n\v\f\r");
	if (!s || !*s)
		return -2;
	free(s);
	int		i;
	long	result;
	(1) && (i = 0), (result = 0);
	while (str[i] == 32 || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '+')
		i++;
	if (!(str[i] >= '0' && str[i] <= '9'))
		return (g_error("error : not a valid option"), -3);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - '0';
		if (result > INT_MAX)
			return (g_error("error surpassed INT MAX"), -2);
		i++;	
	}
	if (str[i] != '\0')
		return (g_error("error : not a valid option"), -2);
	return (result);
}
int		parsing(t_table *table, char **argv)
{
	int arr[6];

	table->ph_nbr = ft_atoi(argv[1]);
	table->time_die = ft_atoi(argv[2]);
	table->time_eat = ft_atoi(argv[3]); 
	table->time_sleep = ft_atoi(argv[4]);
	printf("this is ph_nbr %lu\n", table->ph_nbr);
	if (argv[5])
		table->max_meals = ft_atoi(argv[5]);
	else 
		table->max_meals = -1;
		
	if(!table->ph_nbr)
		return (-1);
	if ((table->time_die == -2) || (table->time_eat == -2) || \
	(table->time_sleep == -2) || table->max_meals == -2)
		return (-1);
	return (0);
}
