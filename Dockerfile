FROM ubuntu:latest
MAINTAINER Venant-Valéry Damien

WORKDIR /compil

RUN apt-get update \
&& apt-get upgrade -yq \
&& apt-get install build-essential make -yq

COPY ./src/. .
COPY ./Makefile .

RUN make

FROM ubuntu:latest
WORKDIR /app

COPY --from=0 /compil/server_web .

CMD ["./server_web"]
