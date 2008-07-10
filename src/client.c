#include "client.h"

double mu_session = 3.86;
double lambda_session = 9.46;
double alfa_tt = 1.4;
double alfa_obj = 1.33;
double mu_html = 7.63;
double sigma_html = 1.001;
double alfa_html = 1;
double mu_emb = 8.215;
double sigma_emb = 1.46;

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
extern CLASS requestClasses[K];

int web_client(double doc_size)
{
	double startTime, endTime, disk_start_time, server_start_time;
	int tmp_server, num_blocks, i;
	startTime = simtime();
	double enterTime = 0.0, server_enterTime = 0.0;
	int tmp_disk = 0;
	//vedi pag. 131 user guide
  note_passage(lambda);
	use(inLink, D_InLink());

	use(CPU_web_switch, D_Cpu(CPU_WEB_SWITCH_SERVICE_RATE)); //cpu_web_switch_speed è ancora da modellare

	use(L2, D_LAN(doc_size));
	
	// implementare qui random, round robin e least_loaded
	/* random */
	tmp_server = csim_random_int(0, NUM_SERVER-1);
	/* fine random */

	server_start_time = enter_box(WebServer);
	use(cpuWS[tmp_server], D_Cpu(CPU_SERVICE_RATE));

	num_blocks = number_of_blocks(doc_size);
	
	//implementare qui quale disco selezionare

	use(diskWS[/*quale disco*/1], num_blocks*D_WSDisk(doc_size));

	use(cpuWS[tmp_server], D_Cpu(CPU_SERVICE_RATE));
	exit_box(WebServer, server_start_time);
	
	use(L2, D_LAN(doc_size)); 

	/*
	caso con il link in più
	  use(L3, D_LAN()); è diversa dalla domanda in entrata???
	*/	
	use(CPU_web_switch, D_Cpu(CPU_WEB_SWITCH_SERVICE_RATE));
	
	use(outLink, D_OutLink(doc_size));
				
	tabulate(rtime, simtime()-startTime);
	
	num_osservazioni++;
	
	/* codice di dammy
	if(transient == TRUE && observed_sample<=maxObservation){
		observations[iterationIndex][observed_sample] = simtime()-startTime;
		observed_sample++;
	}*/
	
	return 0;
}

void web_session(int cli_id, int variant)
{
	char *prova = (char*)malloc(64); //il nome del processo, dovrebbe essere univoco sulla base dell'id
	sprintf(prova,"%d",cli_id);
	create(prova);
	double html_page, emb_obj_size;
	int num_embedded_objects;
	double session = session_request(mu_session, lambda_session);
	int i = 0;
	int j = 0;
	for(; i < session; i++) {
		html_page = html_page_size(mu_html, sigma_html, alfa_html);

		set_process_class(requestClasses[get_doc_class(html_page)]);
		web_client(html_page);
		num_embedded_objects = object_per_request(alfa_obj);
		for(j=0; j < num_embedded_objects; j++) {
			emb_obj_size = embedded_object_size(mu_emb, sigma_emb);
			set_process_class(requestClasses[get_doc_class(emb_obj_size)]);
		}
		hold(user_think_time(alfa_tt)); 

	}
	csim_terminate();
}

int get_doc_class(double doc_size)
{
 double distance[K];
 int i = 0;
 double min;
 int index = 0;
 distance[0] = fabs(10281-doc_size);
 distance[1] = fabs(279513744-doc_size);
 distance[2] = fabs(715827882-doc_size);
 min = distance[0];
 for(i=1; i < K; i++) {
 	if(distance[i] < min) {
 		min = distance[i];
 		index = i;
 	}
 }
 return index;
}
