#include<stdlib.h>
#include<stdio.h>
#include<csim.h>
#include "client.h"
#include "common.h"
#include "gaussiana_inversa.h"
#include "service.h"

extern FACILITY cpuWS[NUM_SERVER];
extern FACILITY diskWS[NUM_DISK];
extern BOX WebServer;
extern FACILITY L2;
extern FACILITY CPU_web_switch;
extern FACILITY inLink;
extern FACILITY outLink;
extern TABLE wsrtime;
extern TABLE rtime;
extern METER lambda;
//decidere quante classi fare sulla base del clustering
extern CLASS requestClasses[4];

extern int num_osservazioni;

TABLE resptime;
double lambda_tmp;


void statistics()
{
/*	double utilizzazione_L2[NUM_CLASSES], utilizzazione_inLink[NUM_CLASSES], utilizzazione_outLink[NUM_CLASSES], 
	       utilizzazione_cpu_web_switch[NUM_CLASSES], utilizzazione_cpu_web_server[NUM_CLASSES], utilizzazione_disco_web_server[NUM_CLASSES];
	double qlen_L2[NUM_CLASSES], qlen_inLink[NUM_CLASSES], qlen_outLink[NUM_CLASSES], qlen_cpu_web_switch[NUM_CLASSES], 
	       qlen_cpu_web_server[NUM_CLASSES], qlen_disco_web_server[NUM_CLASSES]; 
	double rtime_L2[NUM_CLASSES]; rtime_inLink[NUM_CLASSES], rtime_outLink[NUM_CLASSES], rtime_cpu_web_switch[NUM_CLASSES], 
	       rtime_cpu_web_server[NUM_CLASSES], rtime_disco_web_server[NUM_CLASSES];
	
	lambda_tmp += meter_rate(lambda);
	int i = 0;
	// per ogni classe colleziono le statistiche della LAN, dell'inlink, dell'outlink e della cpu del web switch
	for(; i<NUM_CLASSES; i++){ 		
	  utilizzazione_L2[i] += class_util(L2, requestClasses[i]);
		qlen_L2[i] += class_qlen(L2, requestClasses[i]);
		rtime_L2[i] += class_resp(L2, requestClasses[i]);
	
		utilizzazione_inLink[i] += class_util(inLink, requestClasses[i]);
		qLen_inLink[i] += class_qlen(inLink, requestClasses[i]);
		rtime_inLink[i] += class_resp(inLink, requestClasses[i]);
		
		utilizzazione_outLink[i] += class_util(outLink, requestClasses[i]);
		qlen_outLink[i] += class_qlen(outLink, requestClasses[i]);
		rtime_outLink[i] += class_resp(outLink, requestClasses[i]);
		
		utilizzazione_cpu_web_switch[i] += class_util(CPU_web_switch, requestClasses[i]);
		qlen_cpu_web_switch[i] += class_qlen(CPU_web_switch, requestClasses[i]);
		rtime_cpu_web_switch[i] += class_resp(CPU_web_switch, requestClasses[i]);
	}
	
*/	
	
}


void sim(int argc, char **argv) 
{
	int i=0;
	int client_id;
	int variante = 0;
	for(i=0; i<NUM_ITERATIONS; i++){
		num_osservazioni = 0;
		int reset = 0;
		create("simulation");
		max_processes(MAX_PROCESSES);
		max_facilities(MAX_FACILITIES);
		max_servers(MAX_SERVERS);
		max_classes(MAX_CLASSES);
		/*fileName[0] = '\0';
		sprintf(fileName, "Sim_%d_%d", variante, i);
		out = fopen(fileName, "w");
		set_output_file(out);
		
		initStream(); // we initialize all the streams
		*/
		//inizializzazione delle facility
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
	
		resptime = table("System Response Time"); // table intialization
	
		char className[20];
		className[0] = '\0';
	
	//	webSwitch = box("Web Switch");
		WebServer = box("Web Server");
	
		lambda = meter("Arrival Rate");
	
	
		for(i=0; i<4; i++){
			className[0] = '\0';
			sprintf(className, "Classe%d", i);
			requestClasses[i] = process_class(className);
		}

		collect_class_facility_all();
		
		//reseedStream(seed*i);
		while(state(converged)==NOT_OCC && num_osservazioni<400000){
			hold(exponential(1/(double)ARRIVAL));
			web_session(client_id, variante);
			client_id++;
			if(num_osservazioni>100000 &&(!reset)){
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
		statistics(i, variante);
		rerun();
	}
	table_summary();
}
