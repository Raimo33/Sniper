# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/11 18:23:06 by craimond          #+#    #+#              #
#    Updated: 2025/01/13 17:58:57 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

IMAGE_NAME := sniper
CONTAINER_NAME := sniper

all: build run

build:
	docker build -t $(IMAGE_NAME) .

.keys:
	@read -p "Private ed25519 key: " privkey;
	@read -p "API key: " apikey;

run: .keys
	docker run --rm --name $(CONTAINER_NAME) -e PRIV_KEY=$$privkey -e API_KEY=$$apikey $(IMAGE_NAME)

stop:
	docker stop $(CONTAINER_NAME)

clean: stop
	docker rmi $(IMAGE_NAME)

fclean: clean
	sudo docker system prune -a -f --volumes

.PHONY:
.ONESHELL: run
.DEFAULT_GOAL: all
