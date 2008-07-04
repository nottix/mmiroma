#include<math.h>

/* Assunzioni */
#define TCPOV 20 //byte
#define IPOV 20 //byte
#define FRAMEOV 18 //byte

#define MSS 1460
//#define N
//#define S
#define BLOCK_SIZE 2048 //dimensione in byte
#define MBYTE 1048576  //equivalente di un mega espresso in byte
#define LAMBDA 150 //richieste/sec  da parte degli utenti
#define CPU_SERVICE_RATE 150 //richieste/secondo tasso servizio CPU
#define DISK_SEEK_TIME 8.5 //millisecondi
#define CONTROLLER_TIME 0,2 //millisecondi
#define ROTATIONAL_SPEED 7200 //RPM
#define DISK_REVOLUTION_TIME 60/ROTATIONAL_SPEED // sec
#define ROTATIONAL_LATENCY 0.5*DISK_REVOLUTION_TIME //sec 
#define DISK_TRANSFER_RATE 100 //MB/sec
#define BANDWIDTH_L2 1024 //Mbps

//Divide il numero di Byte m in ingresso in datagrammi
int NDatagrams(double m);

//Calcola l'overhead della connessione TCP/IP e del livello DataLink ?????????
int Overhead(double m);

//calcola il numero di blocchi da leggere per un documento
int number_of_blocks(double docSize);

//Calcola il tempo necessario per trasferire m Byte (sec)
//m in byte, bandwidth in bit
double NetworkTime(double m, double bandwidth);

//Domanda di servizio (sec)
double D_InLink();

double D_OutLink(double docSize);

/*Calcola la domanda di servizio sottomessa alla LAN che collega i Web Server con il File Server.
  Ogni richiesta attraversa la LAN per 2 volte: 
  1: comunicazione router -> Web Server (pacchetto HTTP standard)
  2: comunicazione Web Server -> router (dimensione dipendente dalla richiesta)
*/
double D_LAN(double docSize);

//Calcola la domanda di servizio sottomessa ad una CPU (sia Web Switch che web server) (sec)
double D_Cpu(double speed);

//Calcola la domanda di servizio sottomessa ad un disco del Web Server (sec)
double D_WSDisk(double docSize);

