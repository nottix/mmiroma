#include <stdlib.h>
#include <stdio.h>
#include <csim.h>
#include "client.h"
#include "common.h"
#include "workload.h"
#include "service.h"
#include "index.h"

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
extern STREAM p_hit_proxy;

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

extern int num_osservazioni;

TABLE resptime;

//Simulazione dell'intero modello.
void sim(int argc, char **argv) {
	printf("Starting simulation...\n");
	int i=0;
	int client_id;
	int variante = LEAST_LOADED;
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
		reseed(sess_req_1, (int)simtime()+i+1);
		sess_req_2 = create_stream();
		reseed(sess_req_2, (int)simtime()*2+i+1);
		user_tt = create_stream();
		reseed(user_tt, (int)simtime()*3+i+1);
		object_req = create_stream();
		reseed(object_req, (int)simtime()*4+i+1);
		html_1 = create_stream();
		reseed(html_1, (int)simtime()*5+i+1);
		html_2 = create_stream();
		reseed(html_2, (int)simtime()*6+i+1);
		obj_size = create_stream();
		reseed(obj_size, (int)simtime()*7+i+1);
		p_hit_proxy = create_stream();
		reseed(p_hit_proxy, (int)simtime()*8+i+1);
		
		// Inizializzazione delle facility
		inLink = facility("inLink");
		outLink = facility("outLink");
	

		LS1 = facility("LS1");
		LS2 = facility("LS2");
		CPU_web_switch = facility("CPU_web_switch");
		L2 = facility("L2");
		facility_set(cpuWS, "cpuWS", NUM_SERVER);
		facility_set(diskWS, "diskWS", NUM_SERVER*NUM_DISK);
		facility_set(LW2, "LW2", NUM_SERVER);
		
    if(variante == LINK_ADD) {
		 link_add = facility("link_add");
		 facility_set(LW3, "LW3", NUM_SERVER);
		}
		// Table initialization
		rtime = table("System Response Time");
		resptime = permanent_table("Tempo di risposta del sistema");
		char className[20];
		className[0] = '\0';

		WebServer = box("Web Server");
		WebSwitch = box("Web Switch");
		lambda = meter("Arrival Rate");

		int j=0;
		for(; j<NUM_CLASSES; j++){
			className[0] = '\0';
			sprintf(className, "Classe%d", j);
			requestClasses[j] = process_class(className);
		}

		collect_class_facility_all();

		reseed(sess_req_1, (int)simtime()+i+1);
		reseed(sess_req_2, (int)simtime()*2+i+1);
		reseed(user_tt, (int)simtime()*3+i+1);
		reseed(object_req, (int)simtime()*4+i+1);
		reseed(html_1, (int)simtime()*5+i+1);
		reseed(html_2, (int)simtime()*6+i+1);
		reseed(obj_size, (int)simtime()*7+i+1);
		reseed(p_hit_proxy, (int)simtime()*8+i+1);
		
		while(state(converged)==NOT_OCC && num_osservazioni<500000) {
			hold(exponential(1/(double)ARRIVAL));
			if(num_osservazioni%100==0)
				printf("Completamento iterazione %3.2f%%\r", ((double)num_osservazioni/500000)*100);
			web_session(client_id, variante, 0, -1);
			client_id++;
			if(num_osservazioni>100000 &&(!reset)) {
				printf("\nReset statistics %g\n", simtime());
				reset();
				reset=1;
				table_confidence(rtime);
				table_run_length(rtime, 0.005, 0.98, 10000.0);
			}
		}
		printf("Fine generazione a %g - iterazione %d\n", simtime(), i);
		report_facilities();

		report_table(rtime);

		report_boxes();

		meter_summary();
		tabulate(resptime, table_mean(rtime));
		statistics(i, variante);
		rerun();
	}
	table_summary();
}
