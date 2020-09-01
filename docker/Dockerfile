FROM debian:stretch
LABEL maintainer="Digitalcoin Developers <dev@digitalcoin.org>"
LABEL description="Dockerised DigitalcoinCore, built from Travis"

RUN apt-get update && apt-get -y upgrade && apt-get clean && rm -fr /var/cache/apt/*

COPY bin/* /usr/bin/
