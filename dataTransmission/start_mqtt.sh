#!/bin/bash
atsdServer="localhost"
atsdPort="8081"
mqttServer="localhost"

mosquitto -d >/dev/null 2>&1

while true; do
    mosquitto_sub -h $mqttServer -v -t "#" | grep -iE --line-buffered "^iot/([0-9a-z]{8}(-[0-9a-z]{4}){3}-[0-9a-z]{12})/pub\ (debug\ )?series\ e:\1\ .*$" | sed -u "s/^iot\/.*\/pub //" | nc $atsdServer $atsdPort &
    ncPid=$!
    mosquittoSubPid="`expr $ncPid - 3`"
    while kill -0 $ncPid 2>/dev/null; do
	sleep 5
    done
    kill $mosquittoSubPid
    sleep 5
done
