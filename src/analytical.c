#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include "common.h"
#include "service.h"
#define LAMBDA_SIM 3311.86 //prova
#define NUM_OBS 10000000

//Calcola la lunghezza della coda
double calc_queue_length(double utilization)
{
	return utilization/(1-utilization);
}

//Calcola il tempo di risposta
double calc_response_time(double service, double utilization)
{
	return service/(1 - utilization);
}

//Calcola e salva su un file gli indici di prestazione del sistema
int main(int argc, char **argv)
{
	FILE *fd_file;
	char *pathname = (char*)malloc(128);
	sprintf(pathname, "risultati modello analitico.txt");
	fd_file = fopen(pathname, "w");

	double utilizzazione_L2[NUM_CLASSES], utilizzazione_inLink[NUM_CLASSES], utilizzazione_outLink[NUM_CLASSES], 
	utilizzazione_cpu_web_switch[NUM_CLASSES], utilizzazione_cpu_web_server[NUM_CLASSES], utilizzazione_disco_web_server[NUM_CLASSES], utilizzazione_link_add[NUM_CLASSES], utilizzazione_ls1[NUM_CLASSES], utilizzazione_ls2[NUM_CLASSES], utilizzazione_lw2[NUM_CLASSES], utilizzazione_lw3[NUM_CLASSES];

	double coda_L2, coda_inLink, coda_outLink, coda_cpu_web_switch, coda_cpu_web_server, coda_disco_web_server, coda_link_add, coda_ls1, coda_ls2, coda_lw2, coda_lw3;
	double tr_L2, tr_inLink, tr_outLink, tr_cpu_web_switch, tr_cpu_web_server, tr_disco_web_server, tr_link_add, tr_ls1, tr_ls2, tr_lw2, tr_lw3;
	
	//centroidi
	double doc_size[NUM_CLASSES];  
	doc_size[0] = 10281;
	doc_size[1] = 279513744;
	doc_size[2] = 715827882;
	
	//probabilità che la risorsa non si trovi nel proxy
	double miss = 1- P_HIT;    
	int i = 0;
	
	//tasso di richieste per ogni classe = probabilità di classe * tasso di richieste totale
  double lambda[NUM_CLASSES];
	lambda[0] = ((double)9999995/(double)NUM_OBS)*LAMBDA_SIM;
	lambda[1] = ((double)4/(double)NUM_OBS)*LAMBDA_SIM;
	lambda[2] = ((double)1/(double)NUM_OBS)*LAMBDA_SIM;
	fprintf(fd_file, "calcolo indici locali variante standard\n");

	//calcolo utilizzazioni
	for(i=0; i < NUM_CLASSES; i++) {
		fprintf(fd_file, "\nutilizzazioni classe %d\n",i);
		utilizzazione_L2[i] = lambda[i]*D_LAN(doc_size[i]);
		fprintf(fd_file, "L2: %9.9f\n", utilizzazione_L2[i]);

		utilizzazione_inLink[i] = lambda[i]*D_InLink();
		fprintf(fd_file, "inLink: %9.9f\n", utilizzazione_inLink[i]);

		utilizzazione_outLink[i] = lambda[i]*D_OutLink(doc_size[i]);
		fprintf(fd_file, "outLink: %9.9f\n", utilizzazione_outLink[i]);

		utilizzazione_cpu_web_switch[i] = lambda[i] * D_Cpu(CPU_WEB_SWITCH_SERVICE_RATE)* 2; //fattore due perchè processa sia richieste in entrata che in uscita (two-way) 
		fprintf(fd_file, "cpu web switch: %9.9f\n", utilizzazione_cpu_web_switch[i]);

		utilizzazione_cpu_web_server[i] = (lambda[i] * D_Cpu(CPU_SERVICE_RATE) * 2)/(double) NUM_SERVER;
		fprintf(fd_file, "cpu web server: %9.9f\n", utilizzazione_cpu_web_server[i]);

		utilizzazione_disco_web_server[i] = (lambda[i] * D_WSDisk(doc_size[i]))/(NUM_SERVER*NUM_DISK);
		fprintf(fd_file, "disco web server: %9.9f\n", utilizzazione_disco_web_server[i]);
		
		utilizzazione_ls1[i] = lambda[i]*(D_LS1in()+D_LS1out(doc_size[i]));
		fprintf(fd_file, "LS1: %9.9f\n", utilizzazione_ls1[i]);
		utilizzazione_ls2[i] = lambda[i]*D_LAN(doc_size[i]);
		fprintf(fd_file, "LS2: %9.9f\n", utilizzazione_ls2[i]);
		utilizzazione_lw2[i] = (lambda[i]*D_LAN(doc_size[i]))/((double)NUM_SERVER);
		fprintf(fd_file, "LW2: %9.9f\n", utilizzazione_lw2[i]);
	}	      

	//calcolo lunghezza code
	for(i=0; i < NUM_CLASSES; i++) {
		fprintf(fd_file, "\nlunghezze code classe %d\n",i);
		coda_L2 = calc_queue_length(utilizzazione_L2[i]);
		fprintf(fd_file, "L2: %9.9f\n", coda_L2);

		coda_inLink = calc_queue_length(utilizzazione_inLink[i]);
		fprintf(fd_file, "inLink: %9.9f\n", coda_inLink);

		coda_outLink = calc_queue_length(utilizzazione_outLink[i]);
		fprintf(fd_file, "outLink: %9.9f\n", coda_outLink);

		coda_cpu_web_switch = calc_queue_length(utilizzazione_cpu_web_switch[i]);
		fprintf(fd_file, "cpu web switch: %9.9f\n", coda_cpu_web_switch);

		coda_cpu_web_server = calc_queue_length(utilizzazione_cpu_web_server[i]);
		fprintf(fd_file, "cpu web server: %9.9f\n", coda_cpu_web_server);

		coda_disco_web_server = calc_queue_length(utilizzazione_disco_web_server[i]);
		fprintf(fd_file, "disco web server: %9.9f\n", coda_disco_web_server);
		
		coda_ls1 = calc_queue_length(utilizzazione_ls1[i]);
		fprintf(fd_file, "LS1: %9.9f\n", coda_ls1);
		
		coda_ls2 = calc_queue_length(utilizzazione_ls2[i]);
		fprintf(fd_file, "LS2: %9.9f\n", coda_ls2);
		
		coda_lw2 = calc_queue_length(utilizzazione_lw2[i]);
		fprintf(fd_file, "LW2: %9.9f\n", coda_lw2);
	}

	//tempi di residenza
	for(i=0; i < NUM_CLASSES; i++) {
		fprintf(fd_file, "\ntempi di residenza %d\n", i);
		tr_L2 = calc_response_time(D_LAN(doc_size[i]), utilizzazione_L2[i]);
		fprintf(fd_file, "L2: %9.9f\n", tr_L2);

		tr_inLink = calc_response_time(D_InLink(), utilizzazione_inLink[i]);
		fprintf(fd_file, "inLink: %9.9f\n", tr_inLink);

		tr_outLink = calc_response_time(D_OutLink(doc_size[i]), utilizzazione_outLink[i]);
		fprintf(fd_file, "outLink: %9.9f\n", tr_outLink);

		tr_cpu_web_switch = calc_response_time(D_Cpu(CPU_WEB_SWITCH_SERVICE_RATE)* 2, utilizzazione_cpu_web_switch[i]);
		fprintf(fd_file, "cpu web switch: %9.9f\n", tr_cpu_web_switch);

		tr_cpu_web_server = calc_response_time(D_Cpu(CPU_SERVICE_RATE) * 2, utilizzazione_cpu_web_server[i]);
		fprintf(fd_file, "cpu web server: %9.9f\n", tr_cpu_web_server);

		tr_disco_web_server = calc_response_time(D_WSDisk(doc_size[i]), utilizzazione_disco_web_server[i]);
		fprintf(fd_file, "disco web server: %9.9f\n", tr_disco_web_server);
		
		tr_ls1 = calc_response_time((D_LS1in()+D_LS1out(doc_size[i])),utilizzazione_ls1[i]);
		fprintf(fd_file, "LS1: %9.9f\n", tr_ls1);
		
		tr_ls2 = calc_response_time(D_LAN(doc_size[i]),utilizzazione_ls2[i]);		
		fprintf(fd_file, "LS2: %9.9f\n", tr_ls2);
		
		tr_lw2 = calc_response_time(D_LAN(doc_size[i]), utilizzazione_lw2[i]);
		fprintf(fd_file, "LS2: %9.9f\n", tr_lw2);
	}	

	//nel caso di inserimento del proxy bisogna moltiplicare tutte le utilizzazioni per 0.6, nel caso di link addizionale bisogna ricalcolare l'utilizzazione della cpu del web  switch (one way) e calcolare la nuova utilizzazione della L3.
	fprintf(fd_file, "\ncalcolo indici locali con link addizionale\n");

	//calcolo utilizzazioni link addizionale, cosa cambia nella LAN L2?
	for(i=0; i < NUM_CLASSES; i++) {
		fprintf(fd_file, "\nutilizzazioni classe %d\n",i);
		utilizzazione_L2[i] = lambda[i]*D_LAN(0);
		fprintf(fd_file, "L2: %9.9f\n", utilizzazione_L2[i]);

		utilizzazione_inLink[i] = lambda[i]*D_InLink();
		fprintf(fd_file, "inLink: %9.9f\n", utilizzazione_inLink[i]);

		utilizzazione_cpu_web_switch[i] = lambda[i] * D_Cpu(CPU_WEB_SWITCH_SERVICE_RATE);//fattore due perchè processa sia richieste in entrata che in uscita (two-way) 
		fprintf(fd_file, "cpu web switch: %9.9f\n", utilizzazione_cpu_web_switch[i]);

		utilizzazione_cpu_web_server[i] = (lambda[i] * D_Cpu(CPU_SERVICE_RATE) * 2)/(double) NUM_SERVER;
		fprintf(fd_file, "cpu web server: %9.9f\n", utilizzazione_cpu_web_server[i]);

		utilizzazione_disco_web_server[i] = (lambda[i] * D_WSDisk(doc_size[i]))/(NUM_SERVER*NUM_DISK);
		fprintf(fd_file, "disco web server: %9.9f\n", utilizzazione_disco_web_server[i]);

		utilizzazione_link_add[i] = lambda[i]*D_linkAdd(doc_size[i]);
		fprintf(fd_file, "linkAdd: %9.9f\n", utilizzazione_link_add[i]);
		
		utilizzazione_ls1[i] = lambda[i]*(D_LS1in());
		fprintf(fd_file, "LS1: %9.9f\n", utilizzazione_ls1[i]);
		
		utilizzazione_ls2[i] = lambda[i]*D_LAN(0);
		fprintf(fd_file, "LS2: %9.9f\n", utilizzazione_ls2[i]);
		
		utilizzazione_lw2[i] = (lambda[i]*D_LAN(0))/((double)NUM_SERVER);
		fprintf(fd_file, "LW2: %9.9f\n", utilizzazione_lw2[i]);
		
		utilizzazione_lw3[i] = (lambda[i]*D_linkAdd(doc_size[i]))/(double)NUM_SERVER;
		fprintf(fd_file, "LW3: %9.9f\n", utilizzazione_lw3[i]);
	}	      
	//calcolo lunghezza code
	for(i=0; i < NUM_CLASSES; i++) {
		fprintf(fd_file, "\nlunghezze code classe %d\n",i);
		coda_L2 = calc_queue_length(utilizzazione_L2[i]);
		fprintf(fd_file, "L2: %9.9f\n", coda_L2);

		coda_inLink = calc_queue_length(utilizzazione_inLink[i]);
		fprintf(fd_file, "inLink: %9.9f\n", coda_inLink);

		coda_cpu_web_switch = calc_queue_length(utilizzazione_cpu_web_switch[i]);
		fprintf(fd_file, "cpu web switch: %9.9f\n", coda_cpu_web_switch);

		coda_cpu_web_server = calc_queue_length(utilizzazione_cpu_web_server[i]);
		fprintf(fd_file, "cpu web server: %9.9f\n", coda_cpu_web_server);

		coda_disco_web_server = calc_queue_length(utilizzazione_disco_web_server[i]);
		fprintf(fd_file, "disco web server: %9.9f\n", coda_disco_web_server);

		coda_link_add = calc_queue_length(utilizzazione_link_add[i]);
		fprintf(fd_file, "linkAdd: %9.9f\n", coda_link_add);
		
		coda_ls1 = calc_queue_length(utilizzazione_ls1[i]);
		fprintf(fd_file, "LS1: %9.9f\n", coda_ls1);
		
		coda_ls2 = calc_queue_length(utilizzazione_ls2[i]);
		fprintf(fd_file, "LS2: %9.9f\n", coda_ls2);
		
		coda_lw2 = calc_queue_length(utilizzazione_lw2[i]);
		fprintf(fd_file, "LW2: %9.9f\n", coda_lw2);
		
		coda_lw3 = calc_queue_length(utilizzazione_lw3[i]);
		fprintf(fd_file, "LW3: %9.9f\n", coda_lw3);

	}

	//tempi di residenza
	for(i=0; i < NUM_CLASSES; i++) {
		fprintf(fd_file, "\ntempi di residenza %d\n", i);
		tr_L2 = calc_response_time(D_LAN(0), utilizzazione_L2[i]);
		fprintf(fd_file, "L2: %9.9f\n", tr_L2);

		tr_inLink = calc_response_time(D_InLink(), utilizzazione_inLink[i]);
		fprintf(fd_file, "inLink: %9.9f\n", tr_inLink);

		tr_cpu_web_switch = calc_response_time(D_Cpu(CPU_WEB_SWITCH_SERVICE_RATE)* 2, utilizzazione_cpu_web_switch[i]);
		fprintf(fd_file, "cpu web switch: %9.9f\n", tr_cpu_web_switch);

		tr_cpu_web_server = calc_response_time(D_Cpu(CPU_SERVICE_RATE) * 2, utilizzazione_cpu_web_server[i]);
		fprintf(fd_file, "cpu web server: %9.9f\n", tr_cpu_web_server);

		tr_disco_web_server = calc_response_time(D_WSDisk(doc_size[i]), utilizzazione_disco_web_server[i]);
		fprintf(fd_file, "disco web server: %9.9f\n", tr_disco_web_server);
		
		tr_link_add = calc_response_time(D_linkAdd(doc_size[i]), utilizzazione_link_add[i]);
		fprintf(fd_file, "linkAdd: %9.9f\n", tr_link_add);
		
		tr_ls1 = calc_response_time(D_LS1in(), utilizzazione_ls1[i]);
		fprintf(fd_file, "LS1: %9.9f\n", tr_ls1);
		
		tr_ls2 = calc_response_time(D_LAN(0), utilizzazione_ls2[i]);
		fprintf(fd_file, "LS2: %9.9f\n", tr_ls2);
		
		tr_lw2 = calc_response_time(D_LAN(0), utilizzazione_lw2[i]);
		fprintf(fd_file, "LW2: %9.9f\n", tr_lw2);

		tr_lw3 = calc_response_time(D_linkAdd(doc_size[i]), utilizzazione_lw3[i]);
		fprintf(fd_file, "LW3: %9.9f\n", tr_lw3);
		
	}	

	fprintf(fd_file, "\ncalcolo indici locali con proxy\n");

	for(i=0; i < NUM_CLASSES; i++) {
		fprintf(fd_file, "\nutilizzazioni classe %d\n",i);
		utilizzazione_L2[i] = miss*lambda[i]*D_LAN(doc_size[i]);
		fprintf(fd_file, "L2: %9.9f\n", utilizzazione_L2[i]);

		utilizzazione_inLink[i] = miss*lambda[i]*D_InLink();
		fprintf(fd_file, "inLink: %9.9f\n", utilizzazione_inLink[i]);

		utilizzazione_outLink[i] = miss*lambda[i]*D_OutLink(doc_size[i]);
		fprintf(fd_file, "outLink: %9.9f\n", utilizzazione_outLink[i]);

		utilizzazione_cpu_web_switch[i] = miss*lambda[i] * D_Cpu(CPU_WEB_SWITCH_SERVICE_RATE)* 2; //fattore due perchè processa sia richieste in entrata che in uscita (two-way) 
		fprintf(fd_file, "cpu web switch: %9.9f\n", utilizzazione_cpu_web_switch[i]);

		utilizzazione_cpu_web_server[i] = miss*(lambda[i] * D_Cpu(CPU_SERVICE_RATE) * 2)/(double) NUM_SERVER;
		fprintf(fd_file, "cpu web server: %9.9f\n", utilizzazione_cpu_web_server[i]);

		utilizzazione_disco_web_server[i] = miss*(lambda[i] * D_WSDisk(doc_size[i]))/(NUM_SERVER*NUM_DISK);
		fprintf(fd_file, "disco web server: %9.9f\n", utilizzazione_disco_web_server[i]);
		
		utilizzazione_ls1[i] = miss*lambda[i]*(D_LS1in()+D_LS1out(doc_size[i]));
		fprintf(fd_file, "LS1: %9.9f\n", utilizzazione_ls1[i]);
		utilizzazione_ls2[i] = miss*lambda[i]*D_LAN(doc_size[i]);
		fprintf(fd_file, "LS2: %9.9f\n", utilizzazione_ls2[i]);
		utilizzazione_lw2[i] = miss*(lambda[i]*D_LAN(doc_size[i]))/((double)NUM_SERVER);
		fprintf(fd_file, "LW2: %9.9f\n", utilizzazione_lw2[i]);
	}	      
	//calcolo lunghezza code
	for(i=0; i < NUM_CLASSES; i++) {
		fprintf(fd_file, "\nlunghezze code classe %d\n",i);
		coda_L2 = calc_queue_length(utilizzazione_L2[i]);
		fprintf(fd_file, "L2: %9.9f\n", coda_L2);

		coda_inLink = calc_queue_length(utilizzazione_inLink[i]);
		fprintf(fd_file, "inLink: %9.9f\n", coda_inLink);

		coda_outLink = calc_queue_length(utilizzazione_outLink[i]);
		fprintf(fd_file, "outLink: %9.9f\n", coda_outLink);

		coda_cpu_web_switch = calc_queue_length(utilizzazione_cpu_web_switch[i]);
		fprintf(fd_file, "cpu web switch: %9.9f\n", coda_cpu_web_switch);

		coda_cpu_web_server = calc_queue_length(utilizzazione_cpu_web_server[i]);
		fprintf(fd_file, "cpu web server: %9.9f\n", coda_cpu_web_server);

		coda_disco_web_server = calc_queue_length(utilizzazione_disco_web_server[i]);
		fprintf(fd_file, "disco web server: %9.9f\n", coda_disco_web_server);
		
		coda_ls1 = calc_queue_length(utilizzazione_ls1[i]);
		fprintf(fd_file, "LS1: %9.9f\n", coda_ls1);
		
		coda_ls2 = calc_queue_length(utilizzazione_ls2[i]);
		fprintf(fd_file, "LS2: %9.9f\n", coda_ls2);
		
		coda_lw2 = calc_queue_length(utilizzazione_lw2[i]);
		fprintf(fd_file, "LW2: %9.9f\n", coda_lw2);
	}

	//tempi di residenza
	for(i=0; i < NUM_CLASSES; i++) {
		fprintf(fd_file, "\ntempi di residenza %d\n", i);
		tr_L2 = calc_response_time(D_LAN(doc_size[i]), utilizzazione_L2[i]);
		fprintf(fd_file, "L2: %9.9f\n", tr_L2);

		tr_inLink = calc_response_time(D_InLink(), utilizzazione_inLink[i]);
		fprintf(fd_file, "inLink: %9.9f\n", tr_inLink);

		tr_outLink = calc_response_time(D_OutLink(doc_size[i]), utilizzazione_outLink[i]);
		fprintf(fd_file, "outLink: %9.9f\n", tr_outLink);

		tr_cpu_web_switch = calc_response_time(D_Cpu(CPU_WEB_SWITCH_SERVICE_RATE)* 2, utilizzazione_cpu_web_switch[i]);
		fprintf(fd_file, "cpu web switch: %9.9f\n", tr_cpu_web_switch);

		tr_cpu_web_server = calc_response_time(D_Cpu(CPU_SERVICE_RATE) * 2, utilizzazione_cpu_web_server[i]);
		fprintf(fd_file, "cpu web server: %9.9f\n", tr_cpu_web_server);

		tr_disco_web_server = calc_response_time(D_WSDisk(doc_size[i]), utilizzazione_disco_web_server[i]);
		fprintf(fd_file, "disco web server: %9.9f\n", tr_disco_web_server);
		
		tr_ls1 = calc_response_time((D_LS1in()+D_LS1out(doc_size[i])),utilizzazione_ls1[i]);
		fprintf(fd_file, "LS1: %9.9f\n", tr_ls1);
		
		tr_ls2 = calc_response_time(D_LAN(doc_size[i]),utilizzazione_ls2[i]);		
		fprintf(fd_file, "LS2: %9.9f\n", tr_ls2);
		
		tr_lw2 = calc_response_time(D_LAN(doc_size[i]), utilizzazione_lw2[i]);
		fprintf(fd_file, "LS2: %9.9f\n", tr_lw2);
	}	
	fclose(fd_file);
	return 0;
}

void sim(int argc, char **argv) {

}
