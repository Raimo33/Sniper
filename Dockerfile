# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Dockerfile                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/29 17:07:44 by craimond          #+#    #+#              #
#    Updated: 2025/02/07 17:02:22 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#https://github.com/clearlinux/clr-bundles
FROM clearlinux:latest 

RUN swupd update
RUN swupd bundle-add devpkg-openssl devpkg-zlib devpkg-yyjson valgrind

COPY ./build /build

RUN useradd -s /bin/bash fastaf
RUN chown -R fastaf:fastaf /build

WORKDIR /build
RUN cmake .
RUN make -j$(nproc)

# TODO:
# disable_interrupt_coalescing
# disable_gro_lro
# disable RSS
# Set Low-Latency IRQ Affinity

CMD ["./fastaf"]