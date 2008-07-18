CC = gcc
CFLAGS = -g -Wall -std=c99
CSIMPATH = ../lib/csim_c-19.0
INCFLAGS = -I./ -I$(CSIMPATH)/lib
LDFLAGS = -Wl,-rpath,/usr/local/lib 
OBJECTS = gaussiana_inversa.o service.o client.o
LIBS = -lm $(CSIMPATH)/lib/csim.a

all: main

main: main.c $(OBJECTS)
	$(CC) $(INCFLAGS) $(LDFLAGS) $(CFLAGS) -o main main.c $(OBJECTS) $(LIBS)

cluster: cluster.c $(OBJECTS)
	$(CC) $(INCFLAGS) $(LDFLAGS) $(CFLAGS) -o cluster cluster.c $(OBJECTS) $(LIBS)
	
transient: transient.c $(OBJECTS)
	$(CC) $(INCFLAGS) $(LDFLAGS) $(CFLAGS) -o transient transient.c $(OBJECTS) $(LIBS)

analytical: analytical.c $(OBJECTS)
	$(CC) $(INCFLAGS) $(LDFLAGS) $(CFLAGS) -o analytical analytical.c $(OBJECTS) $(LIBS)

######## MODULE ########

client.o: client.h client.c
	$(CC) $(INCFLAGS) $(CFLAGS) -o client.o -c client.c

gaussiana_inversa.o: gaussiana_inversa.h gaussiana_inversa.c
	$(CC) $(INCFLAGS) $(CFLAGS) -o gaussiana_inversa.o -c gaussiana_inversa.c

service.o: service.h service.c
	$(CC) $(INCFLAGS) $(CFLAGS) -o service.o -c service.c

clean:
	rm -rf *.o main cluster

