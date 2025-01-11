# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/19 14:52:58 by egualand          #+#    #+#              #
#    Updated: 2025/01/11 18:41:26 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#TODO add srcs e headers
SRCS := $(addprefix srcs/, 
OBJS := $(SRCS:srcs/%.c=objs/%.o)
HDRS := $(addprefix srcs/headers/, 
INCS := /usr/include srcs/headers

CC := gcc
CFLAGS := -Wall -Wextra -Werror -O3 -static -fomit-frame-pointer -flto
RM := rm -rf

VALGRIND_SUPP := valgrind.supp
VALGRIND_LOG := valgrind.log
VALGRIND_FLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=$(VALGRIND_SUPP) --log-file=$(VALGRIND_LOG)

NAME := sniper

all: init build

build: $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

leaks: all
	valgrind $(VALGRIND_FLAGS) ./$(NAME)

objs/%.o: srcs/%.c $(HDRS)
	$(CC) $(CFLAGS) -I $(INCS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME) $(VALGRIND_LOG)

re: fclean all

.PHONY: all clean fclean re
.SILENT:
.SUFFIXES: .c .o
.IGNORE: clean fclean
.PRECIOUS: %.o
.DEFAULT_GOAL: all