
#include <csim.h>
#define SIMULATION_TIME 500000 //copiato dall'esempio, non so se serve
#define TRANSIENT_TIME 0.02*SIMULATION_TIME //come sopra. 
#define BLOCK_SIZE 2048 //dimensione in byte
#define MBYTE 1048576  //equivalente di un mega espresso in byte
#define ARRIVAL 150 //richieste/sec  da parte degli utenti
#define CPU_SERVICE_RATE 150 //richieste/secondo tasso servizio CPU
#define SEEK_TIME_DISK 8.5 //millisecondi
#define CONTROLLER_TIME 0,2 //millisecondi
#define ROTATIONAL_SPEED 7200 //RPM
#define TRANSFER_RATE 100 //MB/sec
#define BANDWIDTH_L2 1024 // Mbps
#define CPU_WEB_SWITCH_SERVICE_RATE 150 //richieste/secondo tasso servizio CPU switch
#define NUM_SERVER 1 // i punti interrogativi sono messi perchè bisogna trovare il valore corretto
#define NUM_DISK 1 // come sopra

#define MAX_SERVERS 500
#define MAX_PROCESSES 1000000
#define MAX_FACILITIES 3000
#define MAX_CLASSES 10
#define MAX_OBSERVATION 400000#define NUM_CLASSES 4

#define NUM_ITERATIONS 1 //???

FACILITY cpuWS[NUM_SERVER];
FACILITY diskWS[NUM_DISK];
BOX WebServer;
FACILITY L2;
FACILITY CPU_web_switch;
FACILITY inLink;
FACILITY outLink;
TABLE wsrtime;
TABLE rtime;
METER lambda;
CLASS requestClasses[4];
