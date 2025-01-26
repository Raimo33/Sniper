# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/11 18:23:06 by craimond          #+#    #+#              #
#    Updated: 2025/01/26 23:32:28 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

IMAGE_NAME := fast-af
CONTAINER_NAME := fast-af
ENV_FILE := .env
CONTAINER_FLAGS := --net=host --cap-add=SYS_NICE --cap-add=NET_ADMIN --ulimit rtprio=99 --cpuset-cpus=2,3

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
