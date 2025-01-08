# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Dockerfile                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/29 17:07:44 by craimond          #+#    #+#              #
#    Updated: 2025/01/08 18:54:32 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FROM alpine:3.21

SHELL ["/bin/ash", "-c"]

RUN apk add make gcc libuv-dev curl-dev libwebsockets-dev nghttp2-dev ngtcp2-dev brotli-dev openssl-dev yyjson-dev jemalloc-dev

COPY ./build/ /build/

WORKDIR /build
RUN make

#TODO Ed25519 Keys (per REST e FIX (https://developers.binance.com/docs/binance-spot-api-docs/rest-api/endpoint-security-type#ed25519-keys))
ENV API_KEY LOG_LEVEL
USER nobody
ENTRYPOINT ["./arb"]