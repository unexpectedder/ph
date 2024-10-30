# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iazoubi <iazoubi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/09 06:34:46 by iazoubi           #+#    #+#              #
#    Updated: 2024/10/30 08:43:03 by iazoubi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC = cc

CFLAGS = -Wall -Wextra -Werror
CFLAGS = -pthread -g -fsanitize=thread

SRCS = main.c parse.c philo_utils.c init.c moni.c

OBJS = $(SRCS:.c=.o)

TARGET = philo

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	
fclean: clean
	
re: clean all

.PHONY: all clean 