#include<math.h>

/* Assunzioni */
#define TCPOV 20 //byte
#define IPOV 20 //byte
#define FRAMEOV 18 //byte

#define MSS 1460
//#define N
//#define S
#define BANDWIDTH_L2 1024 //Mbps

//Divide il numero di Byte m in ingresso in datagrammi
int NDatagrams(double m);

//Calcola l'overhead della connessione TCP/IP e del livello DataLink ?????????
int Overhead(double m);

//Calcola il tempo necessario per trasferire m Byte (sec)
//m in byte, bandwidth in bit
double NetworkTime(double m, double bandwidth);

//Domanda di servizio (sec)
double D_InLink();

double D_OutLink(double docSize);

//Restituisce la domanda di servizio richiesta al router (sec)
double D_Router(double docSize);

/*Calcola la domanda di servizio sottomessa alla LAN che collega i Web Server con il File Server.
  Ogni richiesta attraversa la LAN per 2 volte: 
  1: comunicazione router -> Web Server (pacchetto HTTP standard)
  2: comunicazione Web Server -> router (dimensione dipendente dalla richiesta)
*/
double D_LAN(double docSize);

//Calcola la domanda di servizio sottomessa ad un Web Server (sec)
double D_WSCpu(double docSize);

//Calcola la domanda di servizio sottomessa ad un disco del Web Server (sec)
double D_FSDisk(double docSize);

