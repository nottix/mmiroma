#include "index.h"

extern FACILITY cpuWS[NUM_SERVER];
extern FACILITY diskWS[NUM_DISK*NUM_SERVER];
extern BOX WebServer;
extern BOX WebSwitch;
extern FACILITY L2;
extern FACILITY CPU_web_switch;
extern FACILITY inLink;
extern FACILITY outLink;
extern FACILITY link_add;
extern FACILITY LS1;
extern FACILITY LS2;
extern FACILITY LW2[NUM_SERVER];
extern FACILITY LW3[NUM_SERVER];
extern TABLE rtime;
extern METER lambda;

extern CLASS requestClasses[K];

double lambda_tmp;
double utilizzazione_L2[NUM_CLASSES], utilizzazione_inLink[NUM_CLASSES], utilizzazione_outLink[NUM_CLASSES], 
utilizzazione_cpu_web_switch[NUM_CLASSES], utilizzazione_cpu_web_server[NUM_CLASSES], utilizzazione_disk[NUM_CLASSES], utilizzazione_link_add[NUM_CLASSES], utilizzazione_ls1[NUM_CLASSES], utilizzazione_ls2[NUM_CLASSES], utilizzazione_lw2[NUM_CLASSES], utilizzazione_lw3[NUM_CLASSES]; 
double qlen_L2[NUM_CLASSES], qlen_inLink[NUM_CLASSES], qlen_outLink[NUM_CLASSES], qlen_cpu_web_switch[NUM_CLASSES], 
qlen_cpu_web_server[NUM_CLASSES], qlen_disk[NUM_CLASSES], qlen_link_add[NUM_CLASSES], qlen_ls1[NUM_CLASSES], qlen_ls2[NUM_CLASSES], qlen_lw2[NUM_CLASSES], qlen_lw3[NUM_CLASSES];
double rtime_L2[NUM_CLASSES], rtime_inLink[NUM_CLASSES], rtime_outLink[NUM_CLASSES], rtime_cpu_web_switch[NUM_CLASSES], 
rtime_cpu_web_server[NUM_CLASSES], rtime_disk[NUM_CLASSES], rtime_link_add[NUM_CLASSES], rtime_ls1[NUM_CLASSES], rtime_ls2[NUM_CLASSES], rtime_lw2[NUM_CLASSES], rtime_lw3[NUM_CLASSES];

double tot_temp;
//Calcolo delle statistiche per ogni componente: utilizzazione, lunghezza delle code e tempo di risposta

void print_tot(FILE *fd)
{
	fprintf(fd, "&%.7f\\\\\n\\hline", tot_temp);
	tot_temp = 0;
}

void statistics(int iteration, int variant) {
	lambda_tmp += meter_rate(lambda);
	int i = 0;
	int j = 0;


	for(; i<NUM_CLASSES; i++) { 		
		utilizzazione_L2[i] += class_util(L2, requestClasses[i]);
		qlen_L2[i] += class_qlen(L2, requestClasses[i]);
		rtime_L2[i] += class_resp(L2, requestClasses[i]);

		utilizzazione_inLink[i] += class_util(inLink, requestClasses[i]);
		qlen_inLink[i] += class_qlen(inLink, requestClasses[i]);
		rtime_inLink[i] += class_resp(inLink, requestClasses[i]);

		utilizzazione_outLink[i] += class_util(outLink, requestClasses[i]);
		qlen_outLink[i] += class_qlen(outLink, requestClasses[i]);
		rtime_outLink[i] += class_resp(outLink, requestClasses[i]);

		utilizzazione_cpu_web_switch[i] += class_util(CPU_web_switch, requestClasses[i]);
		qlen_cpu_web_switch[i] += class_qlen(CPU_web_switch, requestClasses[i]);
		rtime_cpu_web_switch[i] += class_resp(CPU_web_switch, requestClasses[i]);

		utilizzazione_ls1[i] += class_util(LS1, requestClasses[i]);
		qlen_ls1[i] += class_qlen(LS1, requestClasses[i]);
		rtime_ls1[i] += class_resp(LS1, requestClasses[i]);
			
		utilizzazione_ls2[i] += class_util(LS2, requestClasses[i]);
		qlen_ls2[i] += class_qlen(LS2, requestClasses[i]);
		rtime_ls2[i] += class_resp(LS2, requestClasses[i]);
		
		if(variant == LINK_ADD) {
			utilizzazione_link_add[i] += class_util(link_add, requestClasses[i]);
			qlen_link_add[i] += class_qlen(link_add, requestClasses[i]);
			rtime_link_add[i] += class_resp(link_add, requestClasses[i]);
		}
	}

	double util_cpu_tmp[NUM_CLASSES] = {0.0};
	double qLen_cpu_tmp[NUM_CLASSES] = {0.0};
	double rtime_cpu_tmp[NUM_CLASSES] = {0.0};

	double util_disk_tmp[NUM_CLASSES] = {0.0};
	double qLen_disk_tmp[NUM_CLASSES] = {0.0};
	double rtime_disk_tmp[NUM_CLASSES] = {0.0};
	
	double util_lw2_tmp[NUM_CLASSES] = {0.0};
	double qLen_lw2_tmp[NUM_CLASSES] = {0.0};
	double rtime_lw2_tmp[NUM_CLASSES] = {0.0};

	double util_lw3_tmp[NUM_CLASSES] = {0.0};
	double qLen_lw3_tmp[NUM_CLASSES] = {0.0};
	double rtime_lw3_tmp[NUM_CLASSES] = {0.0};

	// Per ogni classe colleziono le statistiche di interesse (mediate sul numero dei server e dei dischi)
	for(j=0; j<NUM_CLASSES; j++) {
		//calcolo metriche cpu web server
		for(i=0; i<NUM_SERVER; i++) {
			util_cpu_tmp[j] += class_util(cpuWS[i], requestClasses[j]);
			qLen_cpu_tmp[j] += class_qlen(cpuWS[i], requestClasses[j]);
			rtime_cpu_tmp[j] += class_resp(cpuWS[i], requestClasses[j]);
			
			util_lw2_tmp[j] += class_util(LW2[i], requestClasses[j]);
			qLen_lw2_tmp[j] += class_qlen(LW2[i], requestClasses[j]);
			rtime_lw2_tmp[j] += class_resp(LW2[i], requestClasses[j]);
			if(variant == LINK_ADD) {
				util_lw3_tmp[j] += class_util(LW3[i], requestClasses[j]);
				qLen_lw3_tmp[j] += class_qlen(LW3[i], requestClasses[j]);
				rtime_lw3_tmp[j] += class_resp(LW3[i], requestClasses[j]);
			
			}
		}
		utilizzazione_cpu_web_server[j] += util_cpu_tmp[j]/(double)NUM_SERVER;
		qlen_cpu_web_server[j] += qLen_cpu_tmp[j]/(double)NUM_SERVER;
		rtime_cpu_web_server[j] += rtime_cpu_tmp[j]/(double)NUM_SERVER;
		
		utilizzazione_lw2[j] += util_lw2_tmp[j]/(double)NUM_SERVER;
		qlen_lw2[j] += qLen_lw2_tmp[j]/(double)NUM_SERVER;
		rtime_lw2[j] += rtime_lw2_tmp[j]/(double)NUM_SERVER;
		if(variant == LINK_ADD) {
			utilizzazione_lw3[j] += util_lw3_tmp[j]/(double)NUM_SERVER;
			qlen_lw3[j] += qLen_lw3_tmp[j]/(double)NUM_SERVER;
			rtime_lw3[j] += rtime_lw3_tmp[j]/(double)NUM_SERVER;
		}
		// calcolo metriche disco
		for(i=0; i<(NUM_SERVER*NUM_DISK); i++) {
			util_disk_tmp[j] += class_util(diskWS[i], requestClasses[j]);
			qLen_disk_tmp[j] += class_qlen(diskWS[i], requestClasses[j]);
			rtime_disk_tmp[j] += class_resp(diskWS[i], requestClasses[j]);
		}
		utilizzazione_disk[j] += util_disk_tmp[j]/(double)(NUM_SERVER*NUM_DISK);
		qlen_disk[j] += qLen_disk_tmp[j]/(double)(NUM_SERVER*NUM_DISK);
		rtime_disk[j] += rtime_disk_tmp[j]/(double)(NUM_SERVER*NUM_DISK);
	}	

	FILE *fd_file;
	char *pathname = "util_qlen_rtime";
	
	//Stampa dei risultati su file
	if(iteration==NUM_ITERATIONS-1) {
		fd_file = fopen(pathname, "w");
		fprintf(fd_file, "\\hline\nUtilizzazioni\n\\hline\n");
		fprintf(fd_file, "Centro &Classe1 &Classe2 &Classe3 &Totale\\\\\n\\hline\n\\hline");
		fprintf(fd_file, "\n cpu web server i-esimo: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", utilizzazione_cpu_web_server[j]/(NUM_ITERATIONS));
			tot_temp += utilizzazione_cpu_web_server[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n disco i-esimo: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", utilizzazione_disk[j]/(NUM_ITERATIONS));
			tot_temp += utilizzazione_disk[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n inLink: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{
			fprintf(fd_file, "&%.7f\t", utilizzazione_inLink[j]/(NUM_ITERATIONS));
			tot_temp += utilizzazione_inLink[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n outLink: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", utilizzazione_outLink[j]/(NUM_ITERATIONS));
			tot_temp += utilizzazione_outLink[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n cpu web switch: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{
			fprintf(fd_file, "&%.7f\t", utilizzazione_cpu_web_switch[j]/(NUM_ITERATIONS));
			tot_temp += utilizzazione_cpu_web_switch[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n LAN: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", utilizzazione_L2[j]/(NUM_ITERATIONS));
			tot_temp +=  utilizzazione_L2[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n LS1: \t");
		for(j=0; j<NUM_CLASSES; j++) 
		{
			fprintf(fd_file, "&%.7f\t", utilizzazione_ls1[j]/(NUM_ITERATIONS));	
			tot_temp += utilizzazione_ls1[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n LS2:\t");
		for(j=0; j<NUM_CLASSES; j++) 
		{	
			fprintf(fd_file, "&%.7f\t", utilizzazione_ls2[j]/(NUM_ITERATIONS));	
			tot_temp += utilizzazione_ls2[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n LW2: \t");
		for(j=0; j<NUM_CLASSES; j++) 
		{
			fprintf(fd_file, "%.7f\t", utilizzazione_lw2[j]/(NUM_ITERATIONS));	
			tot_temp += utilizzazione_lw2[j]/(NUM_ITERATIONS);
		}	
		print_tot(fd_file);
		
		if(variant == LINK_ADD) {
			fprintf(fd_file, "\n LINK_ADD: \t");
			for(j=0; j<NUM_CLASSES; j++)
			{	
				fprintf(fd_file, "&%.7f\t", utilizzazione_link_add[j]/(NUM_ITERATIONS));
				tot_temp += utilizzazione_link_add[j]/(NUM_ITERATIONS);
			}
		  print_tot(fd_file);
		  
			fprintf(fd_file, "\n LW3: \t");
		  for(j=0; j<NUM_CLASSES; j++)
		  { 
				fprintf(fd_file, "&%.7f\t", utilizzazione_lw3[j]/(NUM_ITERATIONS));	
				tot_temp += utilizzazione_lw3[j]/(NUM_ITERATIONS);
			}
			print_tot(fd_file);
						
		}
		
		fprintf(fd_file, "\n\n\\hline\nLunghezza media delle code\n\\hline\n");
		fprintf(fd_file, "Centro &Classe1 &Classe2 &Classe3 &Totale\\\\\n\\hline\n\\hline");
		fprintf(fd_file, "\n cpu web server i-esimo: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{
			fprintf(fd_file, "&%.7f\t", qlen_cpu_web_server[j]/(NUM_ITERATIONS));
			tot_temp += qlen_cpu_web_server[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n disco i-esimo: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", qlen_disk[j]/(NUM_ITERATIONS));
			tot_temp += qlen_disk[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n inLink: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{
			fprintf(fd_file, "&%.7f\t", qlen_inLink[j]/(NUM_ITERATIONS));
			tot_temp += qlen_inLink[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n outLink: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{
			fprintf(fd_file, "&%.7f\t", qlen_outLink[j]/(NUM_ITERATIONS));
			tot_temp += qlen_outLink[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n cpu web switch: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", qlen_cpu_web_switch[j]/(NUM_ITERATIONS));
			tot_temp += qlen_cpu_web_switch[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n LAN: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", qlen_L2[j]/(NUM_ITERATIONS));
			tot_temp += qlen_L2[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n LS1: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{
			fprintf(fd_file, "&%.7f\t", qlen_ls1[j]/(NUM_ITERATIONS));
			tot_temp += qlen_ls1[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n LS2: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{
			fprintf(fd_file, "&%.7f\t", qlen_ls2[j]/(NUM_ITERATIONS));
			tot_temp += qlen_ls2[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n LW2: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{
			fprintf(fd_file, "&%.7f\t", qlen_lw2[j]/(NUM_ITERATIONS));			
    	tot_temp += qlen_lw2[j]/(NUM_ITERATIONS);
    }
    print_tot(fd_file);
    
    if(variant == LINK_ADD) {
			fprintf(fd_file, "\n LINK_ADD: \t");
			for(j=0; j<NUM_CLASSES; j++)
			{
				fprintf(fd_file, "&%.7f\t", qlen_link_add[j]/(NUM_ITERATIONS));
				tot_temp += qlen_link_add[j]/(NUM_ITERATIONS);
			}
			print_tot(fd_file);
			
			fprintf(fd_file, "\n LW3: \t");
	  	for(j=0; j<NUM_CLASSES; j++)
			{
				fprintf(fd_file, "&%.7f\t", qlen_lw3[j]/(NUM_ITERATIONS));
				tot_temp += qlen_lw3[j]/(NUM_ITERATIONS);
			}	
			print_tot(fd_file);
		}
		
		fprintf(fd_file, "\n\n\\hline\nTempo Medio di Risposta\n\\hline\n");
		fprintf(fd_file, "Centro &Classe1 &Classe2 &Classe3 &Totale\\\\\n\\hline\n\\hline");
		fprintf(fd_file, "\n cpu web server i-esimo: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{
			fprintf(fd_file, "&%.7f\t", rtime_cpu_web_server[j]/(NUM_ITERATIONS));
			tot_temp += rtime_cpu_web_server[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n disco i-esimo: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", rtime_disk[j]/(NUM_ITERATIONS));
    	tot_temp += rtime_disk[j]/(NUM_ITERATIONS);
    }
    print_tot(fd_file);
    
		fprintf(fd_file, "\n inLink: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{
			fprintf(fd_file, "&%.7f\t", rtime_inLink[j]/(NUM_ITERATIONS));
			tot_temp += rtime_inLink[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n outlink: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", rtime_outLink[j]/(NUM_ITERATIONS));
			tot_temp += rtime_outLink[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n cpu web switch: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", rtime_cpu_web_switch[j]/(NUM_ITERATIONS));
			tot_temp += rtime_cpu_web_switch[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n LAN: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", rtime_L2[j]/(NUM_ITERATIONS));
			tot_temp += rtime_L2[j]/(NUM_ITERATIONS);
		}
		print_tot(fd_file);
		
		fprintf(fd_file, "\n LS1: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", rtime_ls1[j]/(NUM_ITERATIONS));
    	tot_temp += rtime_ls1[j]/(NUM_ITERATIONS);
    }
    print_tot(fd_file);
    
    fprintf(fd_file, "\n LS2: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{
			fprintf(fd_file, "&%.7f\t", rtime_ls2[j]/(NUM_ITERATIONS));
    	tot_temp += rtime_ls2[j]/(NUM_ITERATIONS);
    }
    print_tot(fd_file);
    
    fprintf(fd_file, "\n LW2: \t");
		for(j=0; j<NUM_CLASSES; j++)
		{	
			fprintf(fd_file, "&%.7f\t", rtime_lw2[j]/(NUM_ITERATIONS));
			tot_temp += rtime_lw2[j]/(NUM_ITERATIONS);
    }
		print_tot(fd_file);
		    
    if(variant == LINK_ADD) {
			fprintf(fd_file, "\n LINK_ADD: \t");
			for(j=0; j<NUM_CLASSES; j++)
			{
				fprintf(fd_file, "&%.7f\t", rtime_link_add[j]/(NUM_ITERATIONS));
				tot_temp += rtime_link_add[j]/(NUM_ITERATIONS);
			}
			print_tot(fd_file);
			
			fprintf(fd_file, "\n LW3: \t");
			for(j=0; j<NUM_CLASSES; j++)
			{
				fprintf(fd_file, "&%.7f\t", rtime_lw3[j]/(NUM_ITERATIONS));
      	tot_temp += rtime_lw3[j]/(NUM_ITERATIONS);
      }
      print_tot(fd_file);
		}
		fprintf(fd_file, "\n\n Tasso medio di arrivi (lambda)                        : %g\n", lambda_tmp/(NUM_ITERATIONS));

		fclose(fd_file);
	}
}
