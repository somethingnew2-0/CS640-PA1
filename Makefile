CC = gcc
FLAGS = -std=gnu99 -g -c -pedantic -Wall

default : pinger.o reflector.o utility.o udp.o packet.o queue.o
	$(CC) -o pinger pinger.o utility.o udp.o packet.o
	$(CC) -o reflector reflector.o utility.o udp.o packet.o queue.o

clean :
	rm -f pinger reflector *.o *~

pinger.o : pinger.c packet.h udp.h utility.h
	$(CC) $(FLAGS) pinger.c
reflector.o : reflector.c packet.h queue.h udp.h utility.h
	$(CC) $(FLAGS) reflector.c
utility.o : utility.c utility.h
	$(CC) $(FLAGS) utility.c
udp.o : udp.c udp.h
	$(CC) $(FLAGS) udp.c
packet.o: packet.c packet.h
	$(CC) $(FLAGS) packet.c
queue.o: queue.c queue.h
	$(CC) $(FLAGS) queue.c
