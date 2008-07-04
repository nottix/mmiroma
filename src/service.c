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


//Calcola il tempo necessario per trasferire m Byte (sec)
//m in byte, bandwidth in bit
double NetworkTime(double m, double bandwidth)
{
 return (double)(8*(m+Overhead(m)))/(double)(1000*1000*bandwidth); //[sec]
}

//Domanda di servizio (sec), dove prendo AvgSizeHTTPRequest e Bandwidth? Di L1 non ho nessuna informazione
double D_InLink()
{
 return 0;
 //return NetworkTime(AvgSizeHTTPRequest, LinkBandwidth) + 2*NetworkTime(0.0001, LinkBandwidth)
}

//manca LinkBandwidth
double D_OutLink(double docSize)
{
 return 0;
 //return NetworkTime(docSize, LinkBandwidth) + 2 * NetworkTime(0.0001, LinkBandwidth) 
}

//Restituisce la domanda di servizio richiesta allo switch (posso modellarlo come un router????) Manca SwitchLatency (sec)
double D_Switch(double docSize)
{
 return 0
 //return (NDatagrams(1024*docSize)+6)* pow(SwitchLatency, -6);
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

//Calcola la domanda di servizio sottomessa ad un Web Server (sec)
double D_WSCpu(double docSize);

//Calcola la domanda di servizio sottomessa ad un disco del Web Server (sec)
double D_WSDisk(double docSize);
