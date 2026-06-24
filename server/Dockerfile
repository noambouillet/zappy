FROM ubuntu:24.04

RUN apt-get update \
	&& apt-get install -y \
        build-essential \
	clang

WORKDIR /app

COPY . .

RUN make zappy_server

COPY server/entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
