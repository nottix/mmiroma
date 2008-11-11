#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include "common.h"
#include "service.h"
#include "index_analytical.h"


#define LAMBDA_SIM 3308.79
#define NUM_OBS 10000000

double utilizzazione_L2[NUM_CLASSES+1], utilizzazione_inLink[NUM_CLASSES+1], utilizzazione_outLink[NUM_CLASSES+1], 
utilizzazione_cpu_web_switch[NUM_CLASSES+1], utilizzazione_cpu_web_server[NUM_CLASSES+1], utilizzazione_disk[NUM_CLASSES+1], utilizzazione_link_add[NUM_CLASSES+1], utilizzazione_ls1[NUM_CLASSES+1], utilizzazione_ls2[NUM_CLASSES+1], utilizzazione_lw2[NUM_CLASSES+1], utilizzazione_lw3[NUM_CLASSES+1];
double qlen_L2[NUM_CLASSES+1], qlen_inLink[NUM_CLASSES+1], qlen_outLink[NUM_CLASSES+1], qlen_cpu_web_switch[NUM_CLASSES+1], 
qlen_cpu_web_server[NUM_CLASSES+1], qlen_disk[NUM_CLASSES+1], qlen_link_add[NUM_CLASSES+1], qlen_ls1[NUM_CLASSES+1], qlen_ls2[NUM_CLASSES+1], qlen_lw2[NUM_CLASSES+1], qlen_lw3[NUM_CLASSES+1];
double rtime_L2[NUM_CLASSES], rtime_inLink[NUM_CLASSES], rtime_outLink[NUM_CLASSES], rtime_cpu_web_switch[NUM_CLASSES], 
rtime_cpu_web_server[NUM_CLASSES], rtime_disk[NUM_CLASSES], rtime_link_add[NUM_CLASSES], rtime_ls1[NUM_CLASSES], rtime_ls2[NUM_CLASSES], rtime_lw2[NUM_CLASSES], rtime_lw3[NUM_CLASSES];


//double coda_L2, coda_inLink, coda_outLink, coda_cpu_web_switch, coda_cpu_web_server, coda_disco_web_server, coda_link_add, coda_ls1, coda_ls2, coda_lw2, coda_lw3;
//double tr_L2, tr_inLink, tr_outLink, tr_cpu_web_switch, tr_cpu_web_server, tr_disco_web_server, tr_link_add, tr_ls1, tr_ls2, tr_lw2, tr_lw3;

//Calcola la lunghezza della coda
double calc_queue_length(double utilization, double util_tot)
{
	return utilization/(1-util_tot);
}

//Calcola il tempo di risposta
double calc_residence_time(double service, double util_tot)
{
	return service/(1 - util_tot);
}

void reset_total()
{
	 utilizzazione_L2[NUM_CLASSES] = 0;
	 utilizzazione_inLink[NUM_CLASSES] = 0; 
	 utilizzazione_outLink[NUM_CLASSES] = 0;
	 utilizzazione_cpu_web_switch[NUM_CLASSES] = 0;
	 utilizzazione_cpu_web_server[NUM_CLASSES] = 0; 
	 utilizzazione_disk[NUM_CLASSES] = 0;
	 utilizzazione_link_add[NUM_CLASSES] = 0;
	 utilizzazione_ls1[NUM_CLASSES] = 0; 
	 utilizzazione_ls2[NUM_CLASSES] = 0; 
	 utilizzazione_lw2[NUM_CLASSES] = 0; 
	 utilizzazione_lw3[NUM_CLASSES] = 0;
   qlen_L2[NUM_CLASSES] = 0;
   qlen_inLink[NUM_CLASSES] = 0;
   qlen_outLink[NUM_CLASSES] = 0;
   qlen_cpu_web_switch[NUM_CLASSES] = 0; 
	 qlen_cpu_web_server[NUM_CLASSES] = 0;
	 qlen_disk[NUM_CLASSES] = 0;
	 qlen_link_add[NUM_CLASSES] = 0; 
	 qlen_ls1[NUM_CLASSES] = 0; 
	 qlen_ls2[NUM_CLASSES] = 0; 
	 qlen_lw2[NUM_CLASSES] = 0; 
	 qlen_lw3[NUM_CLASSES] = 0;
}

void calc_index(int variant)
{
	reset_total();
	int i = 0;
	double miss = 1;
	if(variant == PROXY) //CASO PROXY
	{
			miss = 1-P_HIT;
	}
	//centroidi
	double doc_size[NUM_CLASSES];
	//tasso di richieste per ogni classe = probabilita' di classe * tasso di richieste totale
	double lambda[NUM_CLASSES];
		
	if(K==3) {	
		doc_size[0] = 10281;
		doc_size[1] = 279513744;
		doc_size[2] = 715827882;

		lambda[0] = ((double)9999995/(double)NUM_OBS)*LAMBDA_SIM;
		lambda[1] = ((double)4/(double)NUM_OBS)*LAMBDA_SIM;
		lambda[2] = ((double)1/(double)NUM_OBS)*LAMBDA_SIM;
	}
	else if(K==5) {
		doc_size[0] = 10073;
		doc_size[1] = 17740962;
		doc_size[2] = 70926601;
		doc_size[3] = 279513744;
		doc_size[4] = 715827882;

		lambda[0] = ((double)9999917/(double)NUM_OBS)*LAMBDA_SIM;
		lambda[1] = ((double)65/(double)NUM_OBS)*LAMBDA_SIM;
		lambda[2] = ((double)13/(double)NUM_OBS)*LAMBDA_SIM;
		lambda[3] = ((double)4/(double)NUM_OBS)*LAMBDA_SIM;
		lambda[4] = ((double)1/(double)NUM_OBS)*LAMBDA_SIM;
	}
	
	
	//calcolo utilizzazioni
	for(i=0; i < NUM_CLASSES; i++) {
		
		if(variant != LINK_ADD)
		{
			utilizzazione_L2[i] = miss*lambda[i]*(D_LAN(doc_size[i])+D_LAN(0));
		}
		else
		{
			utilizzazione_L2[i] = lambda[i]*D_LAN(0);
		}
		utilizzazione_inLink[i] = miss*lambda[i]*D_InLink();
			
		if(variant != LINK_ADD ) //STANDARD || PROXY
		{
			utilizzazione_outLink[i] = miss*lambda[i]*D_OutLink(doc_size[i]);
			utilizzazione_cpu_web_switch[i] = miss*lambda[i] * D_Cpu(CPU_WEB_SWITCH_SERVICE_RATE)* 2; //fattore due perche' processa sia richieste in entrata che in uscita (two-way) 
		}
		else if(variant == LINK_ADD)
		{
			utilizzazione_cpu_web_switch[i] = miss*lambda[i] * D_Cpu(CPU_WEB_SWITCH_SERVICE_RATE);
		}
		utilizzazione_cpu_web_server[i] = miss*(lambda[i] * D_Cpu(CPU_SERVICE_RATE) * 2)/(double) NUM_SERVER;

		utilizzazione_disk[i] = miss*(lambda[i] * D_WSDisk(doc_size[i]))/(NUM_SERVER*NUM_DISK);
		
		if(variant != LINK_ADD)
		{
			utilizzazione_ls1[i] = miss*lambda[i]*(D_LS1in()+D_LS1out(doc_size[i]));
			utilizzazione_ls2[i] = miss*lambda[i]*(D_LAN(doc_size[i])+D_LAN(0));
			utilizzazione_lw2[i] = miss*(lambda[i]*(D_LAN(doc_size[i])+D_LAN(0)))/((double)NUM_SERVER);
		}
		else
		{
			utilizzazione_link_add[i] = lambda[i]*D_linkAdd(doc_size[i]);

			utilizzazione_ls1[i] = lambda[i]*(D_LS1in());

		
			utilizzazione_ls2[i] = lambda[i]*D_LAN(0);
		
			utilizzazione_lw2[i] = (lambda[i]*D_LAN(0))/((double)NUM_SERVER);
			utilizzazione_lw3[i] = (lambda[i]*D_linkAdd(doc_size[i]))/(double)NUM_SERVER;
		}
		
	}	      
	//calcolo totale utilizzazioni
	for(i=0; i < NUM_CLASSES; i++)
	{
		utilizzazione_L2[NUM_CLASSES] += utilizzazione_L2[i];
		utilizzazione_inLink[NUM_CLASSES] += utilizzazione_inLink[i];
		if(variant != LINK_ADD)
		{
			utilizzazione_outLink[NUM_CLASSES] += utilizzazione_outLink[i];
		}
		utilizzazione_cpu_web_switch[NUM_CLASSES] +=utilizzazione_cpu_web_switch[i];
		utilizzazione_cpu_web_server[NUM_CLASSES] += utilizzazione_cpu_web_server[i];
		utilizzazione_disk[NUM_CLASSES] += utilizzazione_disk[i];
		utilizzazione_ls1[NUM_CLASSES] += utilizzazione_ls1[i];
		utilizzazione_ls2[NUM_CLASSES] += utilizzazione_ls2[i];
		utilizzazione_lw2[NUM_CLASSES] += utilizzazione_lw2[i];
		if(variant == LINK_ADD)
		{
			utilizzazione_lw3[NUM_CLASSES] += utilizzazione_lw3[i];
			utilizzazione_link_add[NUM_CLASSES] += utilizzazione_link_add[i];
		}
	}
	
	//calcolo lunghezza code
	for(i=0; i < NUM_CLASSES; i++) {
		qlen_L2[i] = calc_queue_length(utilizzazione_L2[i], utilizzazione_L2[NUM_CLASSES]);
		qlen_inLink[i] = calc_queue_length(utilizzazione_inLink[i], utilizzazione_inLink[NUM_CLASSES]);
	
		if(variant != LINK_ADD)
		{
			qlen_outLink[i] = calc_queue_length(utilizzazione_outLink[i], utilizzazione_outLink[NUM_CLASSES]);
		}
		qlen_cpu_web_switch[i] = calc_queue_length(utilizzazione_cpu_web_switch[i], utilizzazione_cpu_web_switch[NUM_CLASSES]);
	
		qlen_cpu_web_server[i] = calc_queue_length(utilizzazione_cpu_web_server[i],utilizzazione_cpu_web_server[NUM_CLASSES]);
	
		qlen_disk[i] = calc_queue_length(utilizzazione_disk[i], utilizzazione_disk[NUM_CLASSES]);
		
		qlen_ls1[i] = calc_queue_length(utilizzazione_ls1[i], utilizzazione_ls1[NUM_CLASSES]);
		
		qlen_ls2[i] = calc_queue_length(utilizzazione_ls2[i], utilizzazione_ls2[NUM_CLASSES]);
		
		qlen_lw2[i] = calc_queue_length(utilizzazione_lw2[i], utilizzazione_lw2[NUM_CLASSES]);
		
		if(variant == LINK_ADD)
		{
			qlen_lw3[i] = calc_queue_length(utilizzazione_lw3[i], utilizzazione_lw3[NUM_CLASSES]);
			qlen_link_add[i] = calc_queue_length(utilizzazione_link_add[i], utilizzazione_link_add[NUM_CLASSES]);
		}
	}
	//calcolo totale qlen
	for(i=0; i < NUM_CLASSES; i++)
	{
		qlen_L2[NUM_CLASSES] += qlen_L2[i];
		qlen_inLink[NUM_CLASSES] += qlen_inLink[i];
		if(variant != LINK_ADD)
		{
			qlen_outLink[NUM_CLASSES] += qlen_outLink[i];
		}
		qlen_cpu_web_switch[NUM_CLASSES] +=qlen_cpu_web_switch[i];
		qlen_cpu_web_server[NUM_CLASSES] += qlen_cpu_web_server[i];
		qlen_disk[NUM_CLASSES] += qlen_disk[i];
		qlen_ls1[NUM_CLASSES] += qlen_ls1[i];
		qlen_ls2[NUM_CLASSES] += qlen_ls2[i];
		qlen_lw2[NUM_CLASSES] += qlen_lw2[i];
		if(variant == LINK_ADD)
		{
			qlen_lw3[NUM_CLASSES] += qlen_lw3[i];
			qlen_link_add[NUM_CLASSES] += qlen_link_add[i];
		}
	}
	

	//tempi di residenza
	for(i=0; i < NUM_CLASSES; i++) {
		
	
	  rtime_inLink[i] = calc_residence_time(D_InLink(), utilizzazione_inLink[NUM_CLASSES]);
		
		if(variant != LINK_ADD)
		{
			rtime_outLink[i] = calc_residence_time(D_OutLink(doc_size[i]), utilizzazione_outLink[NUM_CLASSES]);
		
			rtime_cpu_web_switch[i] = calc_residence_time(D_Cpu(CPU_WEB_SWITCH_SERVICE_RATE)* 2, utilizzazione_cpu_web_switch[NUM_CLASSES]);
	
			rtime_cpu_web_server[i] = calc_residence_time(D_Cpu(CPU_SERVICE_RATE) * 2, utilizzazione_cpu_web_server[NUM_CLASSES]);
			
			rtime_L2[i] = calc_residence_time(D_LAN(doc_size[i]), utilizzazione_L2[NUM_CLASSES]);
			
			rtime_ls1[i] = calc_residence_time((D_LS1in()+D_LS1out(doc_size[i])),utilizzazione_ls1[NUM_CLASSES]);
		
			rtime_ls2[i] = calc_residence_time(D_LAN(doc_size[i]),utilizzazione_ls2[NUM_CLASSES]);		
			
			rtime_lw2[i] = calc_residence_time(D_LAN(doc_size[i]), utilizzazione_lw2[NUM_CLASSES]);
		}
		else
		{
			rtime_L2[i] = calc_residence_time(D_LAN(0), utilizzazione_L2[NUM_CLASSES]);
			rtime_cpu_web_switch[i] = calc_residence_time(D_Cpu(CPU_WEB_SWITCH_SERVICE_RATE), utilizzazione_cpu_web_switch[NUM_CLASSES]);
			rtime_cpu_web_server[i] = calc_residence_time(D_Cpu(CPU_SERVICE_RATE) * 2, utilizzazione_cpu_web_server[NUM_CLASSES]);		
			rtime_ls1[i] = calc_residence_time(D_LS1in(),utilizzazione_ls1[NUM_CLASSES]);
		
			rtime_ls2[i] = calc_residence_time(D_LAN(0), utilizzazione_ls2[NUM_CLASSES]);
		
			rtime_lw2[i] = calc_residence_time(D_LAN(0), utilizzazione_lw2[NUM_CLASSES]);

			rtime_lw3[i] = calc_residence_time(D_linkAdd(doc_size[i]), utilizzazione_lw3[NUM_CLASSES]);
			
			rtime_link_add[i] = calc_residence_time(D_linkAdd(doc_size[i]), utilizzazione_link_add[NUM_CLASSES]);
		}
		
		rtime_disk[i] = calc_residence_time(D_WSDisk(doc_size[i]), utilizzazione_disk[NUM_CLASSES]);

	}	
	
}



//Calcola e salva su un file gli indici di prestazione del sistema
int main(int argc, char **argv)
{
	unlink("risultati_analitico");
	
	calc_index(RANDOM);
	statistics(RANDOM);
	calc_index(LINK_ADD);
	statistics(LINK_ADD);
	calc_index(PROXY);
	statistics(PROXY);

	return 0;
}

void sim(int argc, char **argv) {

}
