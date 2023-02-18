#!/bin/sh

nb_args=$#
args=$1

function startDocker() {
	docker build -t server_web .
	docker run -it -p 8080:8080 server_web
}

if [ $nb_args -ne 1 ]; then
	startDocker
else
	if [ "$args" == "debug" ]; then
		echo "Hey"
		docker build -f DockerfileGdb -t server_web:debug .
		docker run -it -p 8080:8080 server_web:debug
	else
		startDocker
	fi
fi
