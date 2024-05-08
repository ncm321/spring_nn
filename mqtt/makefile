LIBNAME = myMqtt
#LIBNAME2 = sqlite3
#LIBNAME3 = mosquitto
#LIBNAME4 = cjson
APP_NAME = mqtt

LDNAME = LD_LIBRARY_PATH
INSTPATH = `pwd`/lib 
INCLPATH = `pwd`/src

CC = gcc
AR = ar 

all:mqtt_make


mqtt_make:
	${CC} main.c -o main -L ${INSTPATH} -l{LIBNAME} -I ${INCLPATH}
	export ${LDNAME}=:`pwd`/lib && ./main

#-l{LIBNAME2} -l{LIBNAME3} -l{LIBNAME4}

clean:
	@rm ${APP_NAME}
