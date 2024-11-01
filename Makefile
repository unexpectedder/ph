# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iazoubi <iazoubi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/09 06:34:46 by iazoubi           #+#    #+#              #
#    Updated: 2024/10/31 20:51:26 by iazoubi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC = cc

CFLAGS = -Wall -Wextra -Werror #-fsanitize=thread -g
# CFLAGS = -pthread -fsanitize=-fsanitize=thread

SRCS = main.c parse.c philo_utils.c init.c moni.c

OBJS = $(SRCS:.c=.o)

NAME = philo

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(NAME)
	
fclean: clean
	
re: clean all

.PHONY: all clean 