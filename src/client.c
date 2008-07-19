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

extern STREAM p_hit_proxy;

int currentDisk[NUM_SERVER];
int num_osservazioni;
int current_server;
double **observations;
int observed_sample;
int maxObservation;

double client_response_time;

// Ritorna l'indice del server meno utilizzato (serve per la least loaded)
int get_least_loaded() 
{
	int i=0;
	int j=0;
	double disk_qlen = 0.0;
	double server_qlen = 0.0;
	double qlen_tmp = 0.0;
	int index = 0;
	for(i=0; i < NUM_DISK; i++) 
		disk_qlen += qlength(diskWS[i]);

	server_qlen = qlength(cpuWS[0])+disk_qlen+qlength(LW2[0]);
	for(i=1; i<NUM_SERVER; i++) {
		disk_qlen = 0.0;
		for(j=0; j < NUM_DISK; j++) {
			disk_qlen += qlength(diskWS[j+i*NUM_DISK]); //i*NUM_DISK perchè per ogni server ci sono NUM_DISK dischi
		}
		qlen_tmp = disk_qlen+qlength(cpuWS[i])+qlength(LW2[i]);
		if(server_qlen > qlen_tmp) {
			server_qlen = qlen_tmp;
			index = i;
		}
		qlen_tmp = 0.0;
	}
	return index;
}

//Gestisce l'intero flusso richiesta-risposta
int web_client(double doc_size, int variant, int bool_transient, int iter)
{
	double startTime;
	double server_start_time = 0.0;
	double switch_start_time = 0.0;
	int tmp_server;
	startTime = simtime();
	int tmp_disk = 0;
	//vedi pag. 131 user guide
	note_passage(lambda);
	use(inLink, D_InLink());
	
	switch_start_time = enter_box(WebSwitch);
	use(LS1, D_LS1in()); //utilizzato la domanda di inlink perchè la banda è la stessa
	use(CPU_web_switch, D_Cpu(CPU_WEB_SWITCH_SERVICE_RATE)); //cpu_web_switch_speed è ancora da modellare
  use(LS2, D_LAN(0)); //stessa banda della LAN, in richiesta il doc_size è 0
  exit_box(WebSwitch, switch_start_time);
  
	use(L2, D_LAN(0 /*doc_size*/));


	/* random */
	if(variant == RANDOM || variant == LINK_ADD || variant == PROXY) {
		tmp_server = current_server;
		current_server = csim_random_int(0, NUM_SERVER-1);
	}
	else if (variant == ROUND_ROBIN) { //round robin
		tmp_server = current_server;
		current_server = (tmp_server+1)%NUM_SERVER;
	}
	// least loaded
	else if (variant == LEAST_LOADED) {
		tmp_server = get_least_loaded();
	}

	server_start_time = enter_box(WebServer);
	use(LW2[tmp_server], D_LAN(0));
	use(cpuWS[tmp_server], D_Cpu(CPU_SERVICE_RATE));
	

	//selezione del disco round robin
	tmp_disk = currentDisk[tmp_server];
	currentDisk[tmp_server] = (tmp_disk+1)%NUM_DISK;
	use(diskWS[tmp_server*NUM_DISK + tmp_disk], D_WSDisk(doc_size));
	use(cpuWS[tmp_server], D_Cpu(CPU_SERVICE_RATE));
	if(variant != LINK_ADD) {
		use(LW2[tmp_server], D_LAN(doc_size));
	}
	else {
		use(LW3[tmp_server], D_linkAdd(doc_size));
	}		
	exit_box(WebServer, server_start_time);
	if(variant == LINK_ADD) {
		use(link_add, D_linkAdd(doc_size));
	}		  
	else {
		use(L2, D_LAN(doc_size)); 
		use(LS2, D_LAN(doc_size));
		use(CPU_web_switch, D_Cpu(CPU_WEB_SWITCH_SERVICE_RATE));
		use(LS1, D_LS1out(doc_size));
		use(outLink, D_OutLink(doc_size));
	}	
	
	client_response_time = simtime()-startTime;
	
	tabulate(rtime, simtime()-startTime);
	num_osservazioni++;

	//utilizzato per il transiente per tener conto del numero di osservazioni
	if(bool_transient == 1 && observed_sample<=maxObservation){
		observations[iter][observed_sample] = simtime()-startTime;
		observed_sample++;
	}

	return 0;
}

//Generazione di una sessione e delle relative richieste 
void web_session(int cli_id, int variant, int bool_transient, int iter)
{
	char *prova = (char*)malloc(64); //il nome del processo
	sprintf(prova,"%d",cli_id);
	create(prova);
	double html_page, emb_obj_size;
	int num_embedded_objects;
	int session = session_request(mu_session, lambda_session);
	int i = 0;
	int j = 0;
	for(i=0; i < session; i++) {
		html_page = html_page_size(mu_html, sigma_html, alfa_html);

		set_process_class(requestClasses[get_doc_class(html_page)]);
		if(variant == PROXY && stream_prob(p_hit_proxy) > 0.4) { 
			web_client(html_page, variant, bool_transient, iter);
		}
		if(variant != PROXY) {
			web_client(html_page, variant, bool_transient, iter);
		}
		num_embedded_objects = object_per_request(alfa_obj);
		for(j=0; j < num_embedded_objects; j++) {
			emb_obj_size = embedded_object_size(mu_emb, sigma_emb);
			set_process_class(requestClasses[get_doc_class(emb_obj_size)]);
			if(variant == PROXY && stream_prob(p_hit_proxy) > 0.4) {
				web_client(emb_obj_size, variant, bool_transient, iter);
			}
			if(variant != PROXY) {
				web_client(emb_obj_size, variant, bool_transient, iter);
			}
		}
		hold(user_think_time(alfa_tt)); 

	}

	csim_terminate();
}

//assegna una classe ad un documento, sulla base delle distanze da i centroidi
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
