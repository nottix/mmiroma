CC = gcc
CFLAGS = -g -Wall
CSIMPATH = ../lib/csim_c-19.0
INCFLAGS = -I./ -I$(CSIMPATH)/lib
LDFLAGS = -Wl,-rpath,/usr/local/lib $(CSIMPATH)/lib/csim.a
OBJECTS = gaussiana_inversa.o service.o client.o
LIBS = -lm 

all: main cluster

main: main.c client.o gaussiana_inversa.o service.c
	$(CC) $(INCFLAGS) $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o main main.c $(OBJECTS) $(LIBS)

cluster: cluster.c gaussiana_inversa.o
	$(CC) $(INCFLAGS) $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o cluster cluster.c $(OBJECTS) $(LIBS)

######## MODULE ########

client.o: client.h client.c
	$(CC) $(INCFLAGS) $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o client.o -c client.c $(LIBS)

gaussiana_inversa.o: gaussiana_inversa.h gaussiana_inversa.c
	$(CC) $(INCFLAGS) $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o gaussiana_inversa.o -c gaussiana_inversa.c $(LIBS)

service.o: service.h service.c
	$(CC) $(INCFLAGS) $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o service.o -c service.c $(LIBS)

clean:
	rm -rf *.o main cluster
