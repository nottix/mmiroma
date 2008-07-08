CC = gcc
CFLAGS = -g -Wall
INCFLAGS = -I./ -I/usr/local/csim19/lib
LDFLAGS = -Wl,-rpath,/usr/local/lib $* /usr/local/csim19/lib/csim.a
LIBS = -lm 

main: main.c
	$(CC) -o main main.c $(LDFLAGS) $(LIBS) $(INCFLAGS)

