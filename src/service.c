#include "service.h"

//Divide il numero di Byte m in ingresso in datagrammi
int NDatagrams(double m)
{
  int n;
  double f;
  f=m/(double)MSS;	//MSS in Byte
  n=(int)ceilf(f);
  return n;
}

//Calcola l'overhead della connessione TCP/IP e del livello DataLink
int Overhead(double m)
{
 return (NDatagrams(m)*(TCPOV+IPOV+FRAMEOV)); //[Byte]
}

//calcola il numero di blocchi da leggere per un documento
int number_of_blocks(double docSize)
{
 return ceil(docSize/BLOCK_SIZE);
}

//Calcola il tempo necessario per trasferire m Byte (sec)
//m in byte, bandwidth in bit
double NetworkTime(double m, double bandwidth)
{
 return (double)(8*(m+Overhead(m)))/(double)(1000*1000*bandwidth); //[sec]
}

//Domanda di servizio (sec), dove prendo AvgSizeHTTPRequest e Bandwidth? Di L1 non ho nessuna informazione
double D_InLink()
{
 return NetworkTime(AvgSizeHTTPRequest, LinkBandwidth) + 3*NetworkTime(0.0001, LinkBandwidth)
}

//manca LinkBandwidth
double D_OutLink(double docSize)
{
 return NetworkTime(docSize, LinkBandwidth) + 2 * NetworkTime(0.0001, LinkBandwidth) 
}


/*Calcola la domanda di servizio sottomessa alla LAN che collega i Web Server con il File Server.
  Ogni richiesta attraversa la LAN per 2 volte: 
  1: comunicazione router -> Web Server (pacchetto HTTP standard)
  2: comunicazione Web Server -> router (dimensione dipendente dalla richiesta)
*/
double D_LAN(double docSize)
{
 return NetworkTime(AvgSizeHTTPRequest, BANDWIDTH_L2) + NetworkTime(1024 * docSize, BANDWIDTH_L2); //forse moltiplicare per un fattore 2 il secondo Network Time??? BOH
}

//Calcola la domanda di servizio sottomessa ad una CPU (sia Web Switch che web server) (sec), fattore 2 perchè la comunicazione è two way ??? e nel caso con la modifica???
double D_WebSwitchCpu()
{
  return 2 / CPU_WEB_SWITCH_SPEED;
}

//Calcola la domanda di servizio sottomessa ad un disco del Web Server (sec)
double D_WSDisk(double docSize)
{
 NBlocks(docSize) * (DISK_SEEK_TIME + ROTATIONAL_LATENCY + CONTROLLER_TIME + (BLOCK_SIZE/DISK_TRANSFER_RATE));
}
