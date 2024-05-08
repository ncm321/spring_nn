LIBNAME = myMqtt

INSTPATH = `pwd`/../lib 
INCLPATH = `pwd`/../src
INSTALLPATH = `pwd`/../lib 
INCLUDEPATH = `pwd`/
CC = gcc 
AR = ar

all:static_lib
	@make clear
	@make install

static_lib:
	${CC} -c *.c -L ${INSTPATH} -I ${INCLPATH} 
	${AR} -rcs lib${LIBNAME}.a  *.o

install:
	cp lib${LIBNAME}.* ${INSTALLPATH}
#	cp -rf *.h ${INCLUDEPATH}

uninstall:
	rm -f ${INSTPATH}/lib${LIBNAME}.*

clear:
	rm -f *.o

clean:clear
	rm -f lib${LIBNAME}.*
	                          
