# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/11 18:23:06 by craimond          #+#    #+#              #
#    Updated: 2025/01/11 18:39:19 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

IMAGE_NAME := sniper
CONTAINER_NAME := sniper
VOLUME_NAME := sniper_keys

all: build run

build:
	docker volume create $(VOLUME_NAME)
	docker build -t $(IMAGE_NAME) .

run:
	docker run --rm -v $(VOLUME_NAME):/keys --name $(CONTAINER_NAME) $(IMAGE_NAME)

stop:
	docker stop $(CONTAINER_NAME)

clean: stop
	docker rmi $(IMAGE_NAME)

fclean: clean
	sudo docker system prune -a -f --volumes
