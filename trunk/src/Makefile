CC = gcc
CFLAGS = -g -Wall -std=c99
CSIMPATH = ../lib/csim_c-19.0
INCFLAGS = -I./ -I$(CSIMPATH)/lib
LDFLAGS = -Wl,-rpath,/usr/local/lib 
OBJECTS = workload.o service.o client.o index.o 
LIBS = -lm $(CSIMPATH)/lib/csim.a

all: main

main: main.c $(OBJECTS)
	$(CC) $(INCFLAGS) $(LDFLAGS) $(CFLAGS) -o main main.c $(OBJECTS) $(LIBS)

cluster: cluster.c $(OBJECTS)
	$(CC) $(INCFLAGS) $(LDFLAGS) $(CFLAGS) -o cluster cluster.c $(OBJECTS) $(LIBS)
	
transient: transient.c $(OBJECTS)
	$(CC) $(INCFLAGS) $(LDFLAGS) $(CFLAGS) -o transient transient.c $(OBJECTS) $(LIBS)

analytical: analytical.c service.o common.h index_analytical.o
	$(CC) $(INCFLAGS) $(LDFLAGS) $(CFLAGS) -o analytical analytical.c service.o index_analytical.o $(LIBS)

######## MODULE ########

client.o: client.h client.c
	$(CC) $(INCFLAGS) $(CFLAGS) -o client.o -c client.c

workload.o: workload.h workload.c
	$(CC) $(INCFLAGS) $(CFLAGS) -o workload.o -c workload.c

service.o: service.h service.c
	$(CC) $(INCFLAGS) $(CFLAGS) -o service.o -c service.c

index.o: index.h index.c
	$(CC) $(INCFLAGS) $(CFLAGS) -o index.o -c index.c

index_analytical.o: index_analytical.h index_analytical.c
	$(CC) $(INCFLAGS) $(CFLAGS) -o index_analytical.o -c index_analytical.c

clean:
	rm -rf *.o main cluster

