#ifndef _SERVICE_H
#define _SERVICE_H

#include<math.h>
#include "common.h"

#define TCPOV 20 //byte
#define IPOV 20 //byte
#define FRAMEOV 18 //byte
#define MSS 1460
#define AVG_SIZE_HTTP_REQ 290

#define DISK_SEEK_TIME 8.5 //millisecondi
#define DISK_REVOLUTION_TIME 60/(double)ROTATIONAL_SPEED // sec
#define ROTATIONAL_LATENCY 0.5*DISK_REVOLUTION_TIME //sec 
#define DISK_TRANSFER_RATE 100 //MB/sec
/*#define BANDWIDTH_LINKADD 622 //Mbps
#define INLINK_BANDWIDTH 45 //prova
#define OUTLINK_BANDWIDTH 1000 //come sopra
#define LS1_TRANSFER_RATE 1000 
*/

//Divide il numero di Byte m in ingresso in datagrammi
int NDatagrams(double m);

//Calcola l'overhead della connessione TCP/IP e del livello DataLink
int Overhead(double m);

//calcola il numero di blocchi da leggere per un documento
int number_of_blocks(double docSize);

//Calcola il tempo necessario per trasferire m Byte (sec)
//m in byte, bandwidth in bit
double NetworkTime(double m, double bandwidth);

// Domanda di servizio (sec) per l'incoming link
double D_InLink();

//domanda di servizio per l'outgoing link
double D_OutLink(double docSize);

/** 
 * Calcola la domanda di servizio sottomessa alla LAN che collega il Web Switch con i Web Server.
.* Il fattore moltiplicativo 1024 e' commentato perche' le richieste sono gia' espressi in byte
 */
double D_LAN(double docSize);

//Calcola la domanda di servizio sottomessa ad una CPU (sia Web Switch che web server) (sec)
double D_Cpu(double speed);

//Calcola la domanda di servizio sottomessa ad un disco del Web Server (sec)
double D_WSDisk(double doc_size);

//calcola la domanda di servizio del link addizionale
double D_linkAdd(double doc_size);

//calcola la domanda di servizio in entrata della scheda di rete LS1
double D_LS1in();

//calcola la domanda di servizio in uscita della scheda di rete LS1
double D_LS1out(double doc_size);

#endif
