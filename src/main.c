#include <stdlib.h>
#include <stdio.h>
#include <csim.h>
#include "client.h"
#include "common.h"
#include "gaussiana_inversa.h"
#include "service.h"

#ifndef _ISOC99_SOURCE
#define _ISOC99_SOURCE
#endif

extern STREAM sess_req_1;
extern STREAM sess_req_2;
extern STREAM user_tt;
extern STREAM object_req;
extern STREAM html_1;
extern STREAM html_2;
extern STREAM obj_size;

extern FACILITY cpuWS[NUM_SERVER];
extern FACILITY diskWS[NUM_DISK*NUM_SERVER];
extern BOX WebServer;
extern FACILITY L2;
extern FACILITY CPU_web_switch;
extern FACILITY inLink;
extern FACILITY outLink;
extern TABLE rtime;
extern METER lambda;
//decidere quante classi fare sulla base del clustering
extern CLASS requestClasses[K];

extern int num_osservazioni;

TABLE resptime;
double lambda_tmp;
double utilizzazione_L2[NUM_CLASSES], utilizzazione_inLink[NUM_CLASSES], utilizzazione_outLink[NUM_CLASSES], 
utilizzazione_cpu_web_switch[NUM_CLASSES], utilizzazione_cpu_web_server[NUM_CLASSES], utilizzazione_disk[NUM_CLASSES];
double qlen_L2[NUM_CLASSES], qlen_inLink[NUM_CLASSES], qlen_outLink[NUM_CLASSES], qlen_cpu_web_switch[NUM_CLASSES], 
qlen_cpu_web_server[NUM_CLASSES], qlen_disk[NUM_CLASSES]; 
double rtime_L2[NUM_CLASSES], rtime_inLink[NUM_CLASSES], rtime_outLink[NUM_CLASSES], rtime_cpu_web_switch[NUM_CLASSES], 
rtime_cpu_web_server[NUM_CLASSES], rtime_disk[NUM_CLASSES];

void statistics(int iteration) {
	lambda_tmp += meter_rate(lambda);
	int i = 0;
	int j = 0;

	// per ogni classe colleziono le statistiche della LAN, dell'inlink, dell'outlink e della cpu del web switch
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
	}

	double util_cpu_tmp[NUM_CLASSES] = {0.0};
	double qLen_cpu_tmp[NUM_CLASSES] = {0.0};
	double rtime_cpu_tmp[NUM_CLASSES] = {0.0};

	double util_disk_tmp[NUM_CLASSES] = {0.0};
	double qLen_disk_tmp[NUM_CLASSES] = {0.0};
	double rtime_disk_tmp[NUM_CLASSES] = {0.0};
//	for(i=0; i < NUM_CLASSES; i++) {
//		util_cpu_tmp[i] = 0.0;
//		qLen_cpu_tmp[i] = 0.0;
//		rtime_cpu_tmp[i] = 0.0;
//		util_disk_tmp[i] = 0.0;
//		qLen_disk_tmp[i] = 0.0;
//		rtime_disk_tmp[i] = 0.0;
//	}

	// Per ogni classe colleziono le statistiche di interesse (mediate sul numero dei server e dei dischi)
	for(j=0; j<NUM_CLASSES; j++) {
		//calcolo metriche cpu web server
		for(i=0; i<NUM_SERVER; i++){
			util_cpu_tmp[j] += class_util(cpuWS[i], requestClasses[j]);
			qLen_cpu_tmp[j] += class_qlen(cpuWS[i], requestClasses[j]);
			rtime_cpu_tmp[j] += class_resp(cpuWS[i], requestClasses[j]);
		}
		utilizzazione_cpu_web_server[j] += util_cpu_tmp[j]/(double)NUM_SERVER;
		qlen_cpu_web_server[j] += qLen_cpu_tmp[j]/(double)NUM_SERVER;
		rtime_cpu_web_server[j] += rtime_cpu_tmp[j]/(double)NUM_SERVER;

		// calcolo metriche disco
		for(i=0; i<(NUM_SERVER*NUM_DISK); i++){
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
	
	if(iteration==NUM_ITERATIONS-1) {
		fd_file = fopen(pathname, "w");
		fprintf(fd_file, "\n\nUtilizzazione cpu web server i-esimo: \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", utilizzazione_cpu_web_server[j]/(NUM_ITERATIONS));
		fprintf(fd_file, "\nUtilizzazione disco i-esimo              : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", utilizzazione_disk[j]/(NUM_ITERATIONS));
		fprintf(fd_file, "\nUtilizzazione inLink     : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", utilizzazione_inLink[j]/(NUM_ITERATIONS));
		fprintf(fd_file, "\nUtilizzazione outLink     : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", utilizzazione_outLink[j]/(NUM_ITERATIONS));
		fprintf(fd_file, "\nUtilizzazione cpu web switch            : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", utilizzazione_cpu_web_switch[j]/(NUM_ITERATIONS));
		fprintf(fd_file, "\nUtilizzazione LAN                       : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", utilizzazione_L2[j]/(NUM_ITERATIONS));

		fprintf(fd_file, "\n\nLunghezza coda cpu web server i-esimo : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", qlen_cpu_web_server[j]/(NUM_ITERATIONS));
		fprintf(fd_file, "\nLunghezza coda disco i-esimo              : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", qlen_disk[j]/(NUM_ITERATIONS));
		fprintf(fd_file, "\nLunghezza coda inLink      : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", qlen_inLink[j]/(NUM_ITERATIONS));
		fprintf(fd_file, "\nLunghezza coda outLink       : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", qlen_outLink[j]/(NUM_ITERATIONS));
		fprintf(fd_file, "\nLunghezza coda cpu web switch             : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", qlen_cpu_web_switch[j]/(NUM_ITERATIONS));
		fprintf(fd_file, "\nLunghezza coda LAN                        : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", qlen_L2[j]/(NUM_ITERATIONS));

		fprintf(fd_file, "\n\nTempo medio di risposta cpu web server i-esimo: \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", rtime_cpu_web_server[j]/(NUM_ITERATIONS));
		fprintf(fd_file, "\nTempo medio di risposta disco i-esimo             : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", rtime_disk[j]/(NUM_ITERATIONS));

		fprintf(fd_file, "\nTempo medio di risposta inLink     : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", rtime_inLink[j]/(NUM_ITERATIONS));
		fprintf(fd_file, "\nTempo medio di risposta link internet uscita      : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", rtime_outLink[j]/(NUM_ITERATIONS));
		fprintf(fd_file, "\nTempo medio di risposta cpu web switch            : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", rtime_cpu_web_switch[j]/(NUM_ITERATIONS));
		fprintf(fd_file, "\nTempo medio di risposta LAN                       : \t");
		for(j=0; j<NUM_CLASSES; j++)
			fprintf(fd_file, "%.7f\t", rtime_L2[j]/(NUM_ITERATIONS));

		fprintf(fd_file, "\n\n Tasso medio di arrivi (lambda)                        : %g\n", lambda_tmp/(NUM_ITERATIONS));

		fclose(fd_file);
	}
}

//trovare un modo elegante di passare la variante, per ora passo RANDOM
void sim(int argc, char **argv) {
	printf("Starting simulation...\n");
	int i=0;
	int client_id;
	int variante = RANDOM;
	int reset;
	char filename[25];
	FILE *output;
	for(i=0; i<NUM_ITERATIONS; i++) {
		num_osservazioni = 0;
		reset = 0;
		create("simulation");
		max_processes(MAX_PROCESSES);
		max_facilities(MAX_FACILITIES);
		max_servers(MAX_SERVERS);
		max_classes(MAX_CLASSES);
		filename[0] = '\0';
		sprintf(filename, "Sim_prova_%d", i);
		output = fopen(filename, "w");
		set_output_file(output);

		// Inizializzazione degli stream (reseed simtime*i+num)
		sess_req_1 = create_stream();
		reseed(sess_req_1, (int)simtime()+i);
		sess_req_2 = create_stream();
		reseed(sess_req_2, (int)simtime()*2+i);
		user_tt = create_stream();
		reseed(user_tt, (int)simtime()*3+i);
		object_req = create_stream();
		reseed(object_req, (int)simtime()*4+i);
		html_1 = create_stream();
		reseed(html_1, (int)simtime()*5+i);
		html_2 = create_stream();
		reseed(html_2, (int)simtime()*6+i);
		obj_size = create_stream();
		reseed(obj_size, (int)simtime()*7+i);

		// Inizializzazione delle facility
		inLink = facility("inLink");
		outLink = facility("outLink");
		//LS1 = facility("LS1");
		CPU_web_switch = facility("CPU_web_switch");
		//LS2 = facility("LS2");
		L2 = facility("L2");
		//L_add_out = facility("Link-add");
		//facility_set(LW2, "LW2", NUM_SERVER);
		facility_set(cpuWS, "cpuWS", NUM_SERVER);
		facility_set(diskWS, "diskWS", NUM_SERVER*NUM_DISK);
		//facility_set(LW2_out, "LW2_out", NUM_SERVER);

		// Table initialization
		rtime = table("System Response Time");
		resptime = permanent_table("Tempo di risposta del sistema");
		char className[20];
		className[0] = '\0';

		WebServer = box("Web Server");

		lambda = meter("Arrival Rate");

		int j=0;
		for(; j<NUM_CLASSES; j++){
			className[0] = '\0';
			sprintf(className, "Classe%d", j);
			requestClasses[j] = process_class(className);
		}

		collect_class_facility_all();

		reseed(sess_req_1, (int)simtime()+i);
		reseed(sess_req_2, (int)simtime()*2+i);
		reseed(user_tt, (int)simtime()*3+i);
		reseed(object_req, (int)simtime()*4+i);
		reseed(html_1, (int)simtime()*5+i);
		reseed(html_2, (int)simtime()*6+i);
		reseed(obj_size, (int)simtime()*7+i);

		while(state(converged)==NOT_OCC && num_osservazioni<500000) {
			hold(exponential(1/(double)ARRIVAL));
			printf("num_osservazioni %d\n", num_osservazioni);
			web_session(client_id, variante);
			client_id++;
			if(num_osservazioni>100000 &&(!reset)) {
				printf("Reset statistics %g\n", simtime());
				reset();
				reset=1;
				table_confidence(rtime);
				table_run_length(rtime, 0.005, 0.98, 5000.0);
			}
		}
		printf("Fine generazione a %g - iterazione %d\n", simtime(), i);
		report_facilities();

		report_table(rtime);

		report_boxes();

		meter_summary();
		tabulate(resptime, table_mean(rtime));
		statistics(i);
		rerun();
		printf("End %i\n", i);
	}
	table_summary();
}
