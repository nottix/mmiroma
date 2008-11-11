#include "index_analytical.h"

double lambda_tmp;
extern double utilizzazione_L2[NUM_CLASSES], utilizzazione_inLink[NUM_CLASSES], utilizzazione_outLink[NUM_CLASSES], 
utilizzazione_cpu_web_switch[NUM_CLASSES], utilizzazione_cpu_web_server[NUM_CLASSES], utilizzazione_disk[NUM_CLASSES], utilizzazione_link_add[NUM_CLASSES], utilizzazione_ls1[NUM_CLASSES], utilizzazione_ls2[NUM_CLASSES], utilizzazione_lw2[NUM_CLASSES], utilizzazione_lw3[NUM_CLASSES]; 
extern double qlen_L2[NUM_CLASSES], qlen_inLink[NUM_CLASSES], qlen_outLink[NUM_CLASSES], qlen_cpu_web_switch[NUM_CLASSES], 
qlen_cpu_web_server[NUM_CLASSES], qlen_disk[NUM_CLASSES], qlen_link_add[NUM_CLASSES], qlen_ls1[NUM_CLASSES], qlen_ls2[NUM_CLASSES], qlen_lw2[NUM_CLASSES], qlen_lw3[NUM_CLASSES];
extern double rtime_L2[NUM_CLASSES], rtime_inLink[NUM_CLASSES], rtime_outLink[NUM_CLASSES], rtime_cpu_web_switch[NUM_CLASSES], 
rtime_cpu_web_server[NUM_CLASSES], rtime_disk[NUM_CLASSES], rtime_link_add[NUM_CLASSES], rtime_ls1[NUM_CLASSES], rtime_ls2[NUM_CLASSES], rtime_lw2[NUM_CLASSES], rtime_lw3[NUM_CLASSES];

double tot_temp;
//Calcolo delle statistiche per ogni componente: utilizzazione, lunghezza delle code e tempo di risposta



void statistics(int variant) {

	int j = 0;
	FILE *fd_file;
	char *pathname = "risultati_analitico";
	
	//Stampa dei risultati su file
		fd_file = fopen(pathname, "a+");
		fprintf(fd_file, "\\hline\nUtilizzazioni\n\\hline\n");
		fprintf(fd_file, "Centro &Classe1 &Classe2 &Classe3 &Totale\\\\\n\\hline\n\\hline");
		fprintf(fd_file, "\n cpu web server i-esimo: \t");
		for(j=0; j< NUM_CLASSES+1; j++)
		{	
			fprintf(fd_file, "&%.7f\t", utilizzazione_cpu_web_server[j]);
		}

		fprintf(fd_file, "\n disco i-esimo: \t");
		for(j=0; j< NUM_CLASSES+1; j++)
		{	
			fprintf(fd_file, "&%.7f\t", utilizzazione_disk[j]);
		}

		
		fprintf(fd_file, "\n inLink: \t");
		for(j=0; j< NUM_CLASSES+1; j++)
		{
			fprintf(fd_file, "&%.7f\t", utilizzazione_inLink[j]);
		}
		
		fprintf(fd_file, "\n outLink: \t");
		for(j=0; j< NUM_CLASSES+1; j++)
		{	
			fprintf(fd_file, "&%.7f\t", utilizzazione_outLink[j]);
		}

		
		fprintf(fd_file, "\n cpu web switch: \t");
		for(j=0; j< NUM_CLASSES+1; j++)
		{
			fprintf(fd_file, "&%.7f\t", utilizzazione_cpu_web_switch[j]);
		}
		
		fprintf(fd_file, "\n LAN: \t");
		for(j=0; j< NUM_CLASSES+1; j++)
		{	
			fprintf(fd_file, "&%.7f\t", utilizzazione_L2[j]);
		}
		
		fprintf(fd_file, "\n LS1: \t");
		for(j=0; j< NUM_CLASSES+1; j++) 
		{
			fprintf(fd_file, "&%.7f\t", utilizzazione_ls1[j]);
		}
		
		fprintf(fd_file, "\n LS2:\t");
		for(j=0; j< NUM_CLASSES+1; j++) 
		{	
			fprintf(fd_file, "&%.7f\t", utilizzazione_ls2[j]);
		}
		
		fprintf(fd_file, "\n LW2: \t");
		for(j=0; j< NUM_CLASSES+1; j++) 
		{
			fprintf(fd_file, "%.7f\t", utilizzazione_lw2[j]);
		}	
		
		if(variant == LINK_ADD) {
			fprintf(fd_file, "\n LINK_ADD: \t");
			for(j=0; j< NUM_CLASSES+1; j++)
			{	
				fprintf(fd_file, "&%.7f\t", utilizzazione_link_add[j]);
			}
		  
			fprintf(fd_file, "\n LW3: \t");
		  for(j=0; j< NUM_CLASSES+1; j++)
		  { 
				fprintf(fd_file, "&%.7f\t", utilizzazione_lw3[j]);
			}
						
		}
		
		fprintf(fd_file, "\n\n\\hline\nLunghezza media delle code\n\\hline\n");
		fprintf(fd_file, "Centro &Classe1 &Classe2 &Classe3 &Totale\\\\\n\\hline\n\\hline");
		fprintf(fd_file, "\n cpu web server i-esimo: \t");
		for(j=0; j< NUM_CLASSES+1; j++)
		{
			fprintf(fd_file, "&%.7f\t", qlen_cpu_web_server[j]);
		}
		
		fprintf(fd_file, "\n disco i-esimo: \t");
		for(j=0; j< NUM_CLASSES+1; j++)
		{	
			fprintf(fd_file, "&%.7f\t", qlen_disk[j]);
		}
		
		fprintf(fd_file, "\n inLink: \t");
		for(j=0; j< NUM_CLASSES+1; j++)
		{
			fprintf(fd_file, "&%.7f\t", qlen_inLink[j]);
		}
		
		fprintf(fd_file, "\n outLink: \t");
		for(j=0; j< NUM_CLASSES+1; j++)
		{
			fprintf(fd_file, "&%.7f\t", qlen_outLink[j]);
		}
		
		fprintf(fd_file, "\n cpu web switch: \t");
		for(j=0; j< NUM_CLASSES+1; j++)
		{	
			fprintf(fd_file, "&%.7f\t", qlen_cpu_web_switch[j]);
		}
		
		fprintf(fd_file, "\n LAN: \t");
		for(j=0; j< NUM_CLASSES+1; j++)
		{	
			fprintf(fd_file, "&%.7f\t", qlen_L2[j]);
		}
		
		fprintf(fd_file, "\n LS1: \t");
		for(j=0; j< NUM_CLASSES+1; j++)
		{
			fprintf(fd_file, "&%.7f\t", qlen_ls1[j]);
		}
		
		fprintf(fd_file, "\n LS2: \t");
		for(j=0; j< NUM_CLASSES+1; j++)
		{
			fprintf(fd_file, "&%.7f\t", qlen_ls2[j]);
		}
		
		fprintf(fd_file, "\n LW2: \t");
		for(j=0; j< NUM_CLASSES+1; j++)
		{
			fprintf(fd_file, "&%.7f\t", qlen_lw2[j]);
		}
    
		if(variant == LINK_ADD) {
			fprintf(fd_file, "\n LINK_ADD: \t");
			for(j=0; j< NUM_CLASSES+1; j++)
			{
				fprintf(fd_file, "&%.7f\t", qlen_link_add[j]);
			}
			
			fprintf(fd_file, "\n LW3: \t");
		  	for(j=0; j< NUM_CLASSES+1; j++)
			{
				fprintf(fd_file, "&%.7f\t", qlen_lw3[j]);
			}	
		}
		
		fprintf(fd_file, "\n\n\\hline\nTempo Medio di Residenza\n\\hline\n");
		fprintf(fd_file, "Centro &Classe1 &Classe2 &Classe3 &Totale\\\\\n\\hline\n\\hline");
		fprintf(fd_file, "\n cpu web server i-esimo: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{
			fprintf(fd_file, "&%.7f\t", rtime_cpu_web_server[j]);
		}
		
		fprintf(fd_file, "\n disco i-esimo: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", rtime_disk[j]);
    }
    
		fprintf(fd_file, "\n inLink: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{
			fprintf(fd_file, "&%.7f\t", rtime_inLink[j]);
		}
		
		fprintf(fd_file, "\n outlink: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", rtime_outLink[j]);
		}
		
		fprintf(fd_file, "\n cpu web switch: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", rtime_cpu_web_switch[j]);
		}
		
		fprintf(fd_file, "\n LAN: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", rtime_L2[j]);
		}
		
		fprintf(fd_file, "\n LS1: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", rtime_ls1[j]);
		}
    
		fprintf(fd_file, "\n LS2: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{
			fprintf(fd_file, "&%.7f\t", rtime_ls2[j]);
		}
    
		fprintf(fd_file, "\n LW2: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", rtime_lw2[j]);
		}
		    
		if(variant == LINK_ADD) {
			fprintf(fd_file, "\n LINK_ADD: \t");
			for(j=0; j<NUM_CLASSES; j++)
			{
				fprintf(fd_file, "&%.7f\t", rtime_link_add[j]);
			}
			
			fprintf(fd_file, "\n LW3: \t");
			for(j=0; j<NUM_CLASSES; j++)
			{
				fprintf(fd_file, "&%.7f\t", rtime_lw3[j]);
			}
		}

		fclose(fd_file);
	
}
