#ifndef _SERVICE_H
#define _SERVICE_H

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
#define BANDWIDTH_LINKADD 1024 //Mbps
#define AVG_SIZE_HTTP_REQ 2000 //valore a cazzo
#define INLINK_BANDWIDTH 1024 //come sopra
#define OUTLINK_BANDWIDTH 1024 //come sopra

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

double D_LAN(double docSize);

//Calcola la domanda di servizio sottomessa ad una CPU (sia Web Switch che web server) (sec)
double D_Cpu(double speed);

//Calcola la domanda di servizio sottomessa ad un disco del Web Server (sec)
double D_WSDisk(double doc_size);

double D_linkAdd(double doc_size);

#endif
