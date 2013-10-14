CC = gcc
FLAGS = -g -c -pedantic -Wall

default : pinger.o reflector.o utility.o
	$(CC) -o pinger pinger.o utility.o
	$(CC) -o reflector reflector.o utility.o

clean :
	rm -f pinger reflector *.o *~

pinger.o : pinger.c host.h utility.h
	$(CC) $(FLAGS) pinger.c
reflector.o : reflector.c host.h utility.h
	$(CC) $(FLAGS) reflector.c
utility.o : utility.c host.h
	$(CC) $(FLAGS) utility.c
