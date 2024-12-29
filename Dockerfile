# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Dockerfile                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <claudio.raimondi@protonmail.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/29 17:07:44 by craimond          #+#    #+#              #
#    Updated: 2024/12/30 00:52:19 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FROM alpine:3.21

 #TODO install c, compilers etc
SHELL ["/bin/ash", "-c"]

RUN apk add make gcc libuv-dev curl-dev libwebsockets-dev nghttp2-dev ngtcp2-dev brotli-dev openssl-dev zlib-dev yyjson-dev jemalloc-dev

COPY ./build/ /build/

WORKDIR /build
RUN make

USER nobody
ENTRYPOINT ["./arb"]