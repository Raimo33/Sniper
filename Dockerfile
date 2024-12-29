# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Dockerfile                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <claudio.raimondi@protonmail.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/29 17:07:44 by craimond          #+#    #+#              #
#    Updated: 2024/12/29 17:44:07 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FROM alpine:3.21

RUN apk add make libuv-dev libcurl-dev libwebsockets-dev yyjson-dev jemalloc-dev #TODO install c, compilers etc

COPY ./build/ /build/

WORKDIR /build
RUN make

USER nobody
ENTRYPOINT ["./arb"]