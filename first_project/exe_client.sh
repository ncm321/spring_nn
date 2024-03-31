#!/bin/bash

gcc client.c -I ./include -L ./lib -lmysqlite3 -lsqlite3

export LD_LIBRARY_PATH=:`pwd`/lib/ 

./a.out -i 127.0.0.1 -p 30321 -t 3
