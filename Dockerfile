# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Dockerfile                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/29 17:07:44 by craimond          #+#    #+#              #
#    Updated: 2025/01/11 18:50:34 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FROM alpine:3.21

SHELL ["/bin/ash", "-c"]

RUN apk add --no-cache gcc jemalloc-dev wolfssl-dev

COPY ./build/ /build/

WORKDIR /build
VOLUME /keys
RUN make

USER nobody
CMD ["./sniper"]