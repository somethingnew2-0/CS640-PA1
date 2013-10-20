CC = gcc
FLAGS = -std=gnu99 -m32 -g -c -pedantic -Wall

default : pinger.o reflector.o utility.o udp.o packet.o queue.o
	$(CC) -m32 -o pinger pinger.o utility.o udp.o packet.o
	$(CC) -m32 -o reflector reflector.o utility.o udp.o packet.o queue.o

clean :
	rm -f pinger reflector *.o *~

pinger.o : pinger.c host.h utility.h udp.h packet.h
	$(CC) $(FLAGS) pinger.c
reflector.o : reflector.c host.h utility.h udp.h packet.h queue.h
	$(CC) $(FLAGS) reflector.c
utility.o : utility.c host.h
	$(CC) $(FLAGS) utility.c
udp.o : udp.c udp.h
	$(CC) $(FLAGS) udp.c
packet.o: packet.c packet.h
	$(CC) $(FLAGS) packet.c
queue.o: queue.c queue.h
	$(CC) $(FLAGS) queue.c
