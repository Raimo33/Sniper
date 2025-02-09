# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/11 18:23:06 by craimond          #+#    #+#              #
#    Updated: 2025/02/09 09:31:07 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS += -j$(nproc)
IMAGE_NAME := fast-af
CONTAINER_NAME := fast-af
ENV_FILE := .env

# TODO FIX
CONTAINER_FLAGS := --net=host --cap-add=NET_ADMIN --ulimit rtprio=99 --cpuset-cpus=2,3 \
	# --sysctl net.core.rmem_max=16777216 \
	# --sysctl net.core.wmem_max=16777216 \
	# --sysctl net.ipv4.tcp_no_metrics_save=1 \
	# --sysctl net.ipv4.tcp_rmem=4096\ 87380\ 16777216 \
	# --sysctl net.ipv4.tcp_wmem=4096\ 65536\ 16777216 \
	# --sysctl net.core.somaxconn=1024 \
	# --sysctl net.core.netdev_max_backlog=10000 \
	# --sysctl net.core.optmem_max=40960 \
	# --sysctl net.ipv4.tcp_fastopen=3 \
	# --sysctl net.ipv4.tcp_slow_start_after_idle=0 \
	# --sysctl net.ipv4.ip_local_port_range=1024\ 65535 \
	# --sysctl net.ipv4.tcp_window_scaling=1 \
	# --sysctl net.ipv4.tcp_timestamps=1 \
	# --sysctl net.ipv4.tcp_sack=1 \
	# --sysctl net.ipv4.tcp_max_syn_backlog=4096 \
	# --sysctl net.ipv4.tcp_congestion_control=bbr \
	# --sysctl net.ipv4.tcp_max_orphans=65536 \
	# --sysctl net.ipv4.tcp_fin_timeout=5 \
	# --sysctl net.ipv4.tcp_keepalive_time=30 \
	# --sysctl net.ipv4.tcp_keepalive_probes=3 \
	# --sysctl net.ipv4.tcp_keepalive_intvl=5 \
	# --sysctl net.ipv4.tcp_syncookies=0 \
	# --sysctl net.ipv4.conf.all.accept_redirects=0 \
	# --sysctl net.ipv4.conf.all.send_redirects=0 \
	# --sysctl net.ipv4.ip_forward=0 \

all: build run

build:
	docker build -t $(IMAGE_NAME) .

run:
	docker run --rm $(CONTAINER_FLAGS) --name $(CONTAINER_NAME) --env-file $(ENV_FILE) $(IMAGE_NAME)

stop:
	docker stop $(CONTAINER_NAME)

clean: stop
	docker rmi $(IMAGE_NAME)

fclean: clean
	sudo docker system prune -a -f --volumes

re: fclean all

.PHONY: all build run stop clean fclean re
.IGNORE: clean fclean stop
.ONESHELL: run
.DEFAULT_GOAL: all
