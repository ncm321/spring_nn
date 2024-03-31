#!/bin/bash 

gcc server.c -I ../include -L ../lib -lmysqlite3 -lsqlite3

export LD_LIBRARY_PATH=`pwd`/../lib/ 

./a.out -p 30321
