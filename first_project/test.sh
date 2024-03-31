#!/bin/bash

gcc -shared -fPIC -o libmysqlite3.so mysqlite3.c -I ./include/ 

