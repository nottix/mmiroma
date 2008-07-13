#ifndef _COMMON_H
#define _COMMON_H

#include <csim.h>

#define _ISOC99_SOURCE

#define BLOCK_SIZE 2048 //dimensione in byte
#define MBYTE 1048576  //equivalente di un mega espresso in byte
#define ARRIVAL 150 //richieste/sec  da parte degli utenti
#define CPU_SERVICE_RATE 150 //richieste/secondo tasso servizio CPU
#define SEEK_TIME_DISK 8.5 //millisecondi
#define CONTROLLER_TIME 0.2 //millisecondi
#define ROTATIONAL_SPEED 7200 //RPM
#define TRANSFER_RATE 100 //MB/sec
#define BANDWIDTH_L2 1024 // Mbps
#define CPU_WEB_SWITCH_SERVICE_RATE 15000 //richieste/secondo tasso servizio CPU switch
#define NUM_SERVER 200 // i punti interrogativi sono messi perchè bisogna trovare il valore corretto
#define NUM_DISK 20 // come sopra
#define P_HIT 0.4 

#define MAX_SERVERS 5000
#define MAX_PROCESSES 100000000
#define MAX_FACILITIES 5000
#define MAX_CLASSES 10
#define MAX_OBSERVATION 400000

#define K 3
#define NUM_ITERATIONS 1 //???
#define SEED 3

FACILITY cpuWS[NUM_SERVER];
FACILITY diskWS[NUM_DISK*NUM_SERVER];
BOX WebServer;
FACILITY L2;
FACILITY CPU_web_switch;
FACILITY inLink;
FACILITY outLink;
TABLE wsrtime;
TABLE rtime;
METER lambda;
CLASS requestClasses[K];

#endif