#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "service.h"

double calc_queue_length(double utilization)
{
 return utilization/(1-utilization);
}

double calc_response_time(double service, double utilization)
{
 return service/(1 - utilization);
}

int main(int argc, char **argv)
{
 double utilizzazione_L2[NUM_CLASSES], utilizzazione_inLink[NUM_CLASSES], utilizzazione_outLink[NUM_CLASSES], 
	      utilizzazione_cpu_web_switch[NUM_CLASSES], utilizzazione_cpu_web_server[NUM_CLASSES], utilizzazione_disco_web_server[NUM_CLASSES];
 
 double coda_L2, coda_inLink, coda_outLink, coda_cpu_web_switch, coda_cpu_web_server, coda_disco_web_server;    
 double tr_L2[NUM_CLASSES], tr_inLink[NUM_CLASSES], tr_outLink[NUM_CLASSES], 
	      tr_cpu_web_switch[NUM_CLASSES], tr_cpu_web_server[NUM_CLASSES], tr_disco_web_server[NUM_CLASSES];
 double doc_size[NUM_CLASSES];      
 int i = 0;
 //calcolo utilizzazioni
 for(i=0; i < NUM_cLASSES; i++) {
 	utilizzazione_L2[i] = lambda[i]*D_LAN(doc_size[i]);
 	utilizzazione_inLink[i] = lambda[i]*D_inLink();
 	utilizzazione_outLink[i] = lambda[i]*D_outLink(doc_size[i]);
 	utilizzazione_cpu_web_switch[i] = lambda[i] * D_CPU(CPU_WEB_SWITCH_SERVICE_RATE)* 2 //fattore due perchè processa sia richieste in entrata che in uscita (two-way) 
 	utilizzazione_cpu_web_server[i] = (lambda[i] * D_CPU(CPU_SERVICE_RATE) * 2)/(double) NUM_SERVER;
 	utilizzazione_disco_web_server[i] = (lambda[i] * D_WSDisk())/(NUM_SERVER*NUM_DISK);
 }	      
 
 //calcolo lunghezza code
 for(i=0; i < NUM_CLASSES; i++) {
 	coda_L2 = calc_queue_length(utilizzazione_L2[i]);
 	coda_inLink += calc_queue_length(utilizzazione_inLink[i]; 	
 	coda_outLink += calc_queue_length(utilizzazione_outLink[i]);
 	coda_cpu_web_switch += calc_queue_length(utilizzazione_cpu_web_switch[i]);
 	coda_cpu_web_server += calc_queue_length(utilizzazione_cpu_web_server[i]);
 	coda_disco_web_server += calc_queue_length(utilizzazione_disco_web_server[i]);
 }
 //tempi di residenza
  for(i=0; i < NUM_cLASSES; i++) {
 	tr_L2[i] = calc_response_time(D_LAN(doc_size[i]), utilizzazione_L2[i]);
 	tr_inLink[i] = calc_response_time(D_inLink(), utilizzazione_inLink[i]);
 	tr_outLink[i] = calc_response_time(D_outLink(doc_size[i]), utilizzazione_outLink[i]);
 	tr_cpu_web_switch[i] = calc_response_time(D_CPU(CPU_WEB_SWITCH_SERVICE_RATE)* 2, utilizzazione_cpu_web_switch[i]);
 	tr_cpu_web_server[i] = calc_response_time(D_CPU(CPU_SERVICE_RATE) * 2, utilizzazione_cpu_web_server[i]);
 	tr_disco_web_server[i] = calc_response_time(D_WSDisk(), utilizzazione_disco_web_server[i]);
 }	
 
 //nel caso di inserimento del proxy bisogna moltiplicare tutte le utilizzazioni per 0.6 (in teoria si potrebbe evitare di scrivere nel codice e metterlo direttamente in un  
 //file excel, nel caso di link addizionale bisogna ricalcolare l'utilizzazione della cpu del web switch dividendola per 2 e calcolare la nuova utilizzazione della L3.
 
  
}
