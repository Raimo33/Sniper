# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/19 14:52:58 by craimond          #+#    #+#              #
#    Updated: 2025/01/30 18:57:09 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := FastAF

SRCS_DIR := srcs
HDRS_DIR := srcs/headers
INCS_DIR := /usr/include

SRCS := $(addprefix $(SRCS_DIR)/, fastaf.c ws.c fix.c rest.c dns_resolver.c event_loop.c http_parser.c logger.c signals.c errors.c keys.c ssl.c fixed_point.c message_broker.c str_utils.c)
HDRS := $(addprefix $(HDRS_DIR)/, fastaf.h ws.h fix.h rest.h dns_resolver.h event_loop.h http_parser.h logger.h signals.h errors.h keys.h ssl.h fixed_point.h message_broker.h str_utils.h extensions.h)
INCS := -I$(HDRS_DIR) -I$(INCS_DIR)
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)

CC := gcc
CSTANDARD := gnu23
CFLAGS := -std=$(CSTANDARD) -Wall -Wextra -Werror -fwrapv -static -Ofast -ffast-math -fopt-info-vec-all -march=native -fomit-frame-pointer -flto -MMD
LDFLAGS := -static -lssl -lcrypto 
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

all: build

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

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(DEPS) $(VALGRIND_LOG) $(CACHEGRIND_LOG) $(CALLGRIND_LOG) $(MASSIF_LOG)

fclean: clean
	$(RM) $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: clean fclean re
.SILENT: all init build leaks memory clean fclean re
.SUFFIXES: .c .o
.IGNORE: clean fclean re leaks memory
.PRECIOUS: $(OBJS)
.NOTPARALLEL: leaks cache
.ONESHELL: leaks memory
.DEFAULT_GOAL: all