#include "service.h"

//Divide il numero di Byte m in ingresso in datagrammi
int NDatagrams(double m)
{
	int n;
	double f;
	f=m/(double)MSS;	//MSS in Byte
	n=(int)ceil(f);
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
	return NetworkTime(AVG_SIZE_HTTP_REQ, INLINK_BANDWIDTH) + 3*NetworkTime(0.0001, INLINK_BANDWIDTH);
}

//manca LinkBandwidth
double D_OutLink(double docSize)
{
	return NetworkTime(docSize, OUTLINK_BANDWIDTH) + 2 * NetworkTime(0.0001, OUTLINK_BANDWIDTH); 
}


/*Calcola la domanda di servizio sottomessa alla LAN che collega i Web Server con il File Server.
  Ogni richiesta attraversa la LAN per 2 volte: 
  1: comunicazione switch -> Web Server (pacchetto HTTP standard)
  2: comunicazione Web Server -> switch (dimensione dipendente dalla richiesta). Il fattore moltiplicativo 1024 è commentato perchè le richieste sono già espressi in byte
 */
double D_LAN(double docSize)
{
	return NetworkTime(AVG_SIZE_HTTP_REQ, BANDWIDTH_L2) + NetworkTime(/*1024 **/ docSize, BANDWIDTH_L2);
}

//Calcola la domanda di servizio sottomessa ad una CPU (sia Web Switch che web server) (sec), 
double D_Cpu(double speed)
{
	return 1 / speed;
}

//Calcola la domanda di servizio sottomessa ad un disco del Web Server (sec)
double D_WSDisk(double doc_size)
{
	double ret = (number_of_blocks(doc_size)) * ((DISK_SEEK_TIME/pow(10,3)) + ROTATIONAL_LATENCY + (CONTROLLER_TIME/pow(10,3)) + (BLOCK_SIZE/((double)DISK_TRANSFER_RATE*pow(10,6))));
	return ret;
}

double D_linkAdd(double doc_size)
{
	return NetworkTime(doc_size, BANDWIDTH_LINKADD) + 2 * NetworkTime(0.0001, BANDWIDTH_LINKADD); 
}
