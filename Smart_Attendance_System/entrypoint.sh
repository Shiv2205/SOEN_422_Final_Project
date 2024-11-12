#!/bin/bash

#Background MS SQL Process
/opt/mssql/bin/sqlservr &

#Start up delay
sleep 10

#Run init.sql to create and populate DB
/opt/mssql-tools18/bin/sqlcmd -S localhost -U sa -P 'MySecureP@SOEN_422' -i /docker-entrypoint-initdb.d/init.sql -C

#Wait for server to exit
wait
