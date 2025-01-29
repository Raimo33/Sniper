# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/19 14:52:58 by egualand          #+#    #+#              #
#    Updated: 2025/01/29 21:33:16 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := FastAF

OBJS_DIR := objs
SRCS_DIR := srcs
HDRS_DIR := srcs/headers
INCS_DIR := /usr/include

SRCS := $(addprefix $(SRCS_DIR)/, fastaf.c ws.c fix.c rest.c dns_resolver.c event_loop.c http_parser.c logger.c signals.c errors.c keys.c ssl.c fixed_point.c message_broker.c str_utils.c)
OBJS := $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
HDRS := $(addprefix $(HDRS_DIR)/, fastaf.h ws.h fix.h rest.h dns_resolver.h event_loop.h http_parser.h logger.h signals.h errors.h keys.h ssl.h fixed_point.h message_broker.h str_utils.h extensions.h)
INCS := $(addprefix -I, $(INCS_DIR))

CC := gcc
CVERSION := 23
CFLAGS := -std=c$(CVERSION) -Wall -Wextra -Werror -fwrapv -static -Ofast -ffast-math -fopt-info-vec-all -march=native -static -fomit-frame-pointer -flto
LDFLAGS := -static -lwolfssl -lwolfcrypt
RM := rm -rf

#TODO profiling con perf

VALGRIND_SUPP := valgrind.supp
VALGRIND_LOG := valgrind.log
CACHEGRIND_LOG := cachegrind.log
CALLGRIND_LOG := callgrind.log
MASSIF_LOG := massif.log

VALGRIND_FLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=$(VALGRIND_SUPP) --log-file=$(VALGRIND_LOG)
CACHEGRIND_FLAGS := --tool=cachegrind --cache-sim=yes --branch-sim=yes --dump-instr=yes --cachegrind-out-file=$(CACHEGRIND_LOG)
CALLGRIND_FLAGS := --tool=callgrind --callgrind-out-file=$(CALLGRIND_LOG)
MASSIF_FLAGS := --tool=massif --massif-out-file=$(MASSIF_LOG)

$(NAME): all

all: init build

init:
	mkdir -p objs

build: $(OBJS)
	$(CC) $(CFLAGS) $(INCS) $(OBJS) -o $(NAME) $(LDFLAGS)

leaks: all
	valgrind $(VALGRIND_FLAGS) ./$(NAME)

memory: all
	valgrind $(CACHEGRIND_FLAGS) ./$(NAME)
	cg_annotate $(CACHEGRIND_LOG)
	valgrind $(CALLGRIND_FLAGS) ./$(NAME)
	callgrind_annotate $(CALLGRIND_LOG)
	valgrind $(MASSIF_FLAGS) ./$(NAME)
	ms_print $(MASSIF_LOG)

objs/%.o: srcs/%.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(VALGRIND_LOG) $(CACHEGRIND_LOG) $(CALLGRIND_LOG) $(MASSIF_LOG)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re
.SILENT: all init build leaks memory clean fclean re
.SUFFIXES: .c .o
.IGNORE: clean fclean re leaks memory
.PRECIOUS: %.o
.NOTPARALLEL: leaks cache
.ONESHELL: leaks memory
.DEFAULT_GOAL: all