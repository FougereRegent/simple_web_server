#!/bin/sh


docker build -t server_web .
docker run -it -p 8080:8080 server_web
