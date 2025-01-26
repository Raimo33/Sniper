# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Dockerfile                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/29 17:07:44 by craimond          #+#    #+#              #
#    Updated: 2025/01/27 00:00:14 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FROM clearlinux:latest 

RUN swupd update
RUN swupd bundle-add make c-basic os-core-dev
#TODO wolfssl da source

CMD ["tail", "-f", "/dev/null"] 

# COPY ./conf/sysctl.conf /etc/sysctl.conf
# COPY ./build /build

# RUN useradd -s /bin/bash fastaf
# RUN chown -R fastaf:fastaf /build

# WORKDIR /build
# RUN make

# ENTRYPOINT ["sysctl", "-p"]
# CMD ["chrt", "-f", "99", "./fastAF"]