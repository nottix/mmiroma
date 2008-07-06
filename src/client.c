#include "client.h"

int web_client(double doc_size)
{
	double startTime, endTime, disk_start_time, server_start_time;
	int tmp_server, num_blocks, i;
	startTime = simtime();
	double enterTime = 0.0, server_enterTime = 0.0;
	int tmp_disk = 0;

	use(inLink, DinLink());

	use(CPU_web_switch, D_cpu(cpu_web_switch_speed)); //cpu_web_switch_speed è ancora da modellare

	use(L2, D_LAN(doc_size));
	
	// implementare qui random, round robin e least_loaded

	use(cpu_WS[tmp_server], D_CPU(CPU_SERVICE_RATE));

	num_blocks = getNumBlocks(doc_size);
	
	//implementare qui quale disco selezionare
	
	use(disk_WS[/*quale disco*/], num_blocks*D_WSDisk());

	use(cpu_WS[tmp_server], DCpuWebServer(CPU_SERVICE_RATE));
	
	//use(L2, D_LAN()); è diversa dalla domanda in entrata???

	/*
	caso con il link in più
	  use(L3, D_LAN()); è diversa dalla domanda in entrata???
	*/	
	use(CPU_web_switch, D_Cpu(cpu_web_switch_speed));
	
	use(outLink, D_OutLink(doc_size));
				
	tabulate(respTime, simtime()-startTime);
	
	num_osservazioni++;
	
	/*
	if(transientBoolean == TRUE && observed_sample<=maxObservation){
		observations[iterationIndex][observed_sample] = simtime()-startTime;
		observed_sample++;
	}*/
	
	return 0;
}

//dove devo mettere il think time?
void web_session(int cli_id, int variant)
{
	char *prova = "prova"; //il nome del processo, dovrebbe essere univoco sulla base dell'id
	create("prova");
	double html_page, embedded_object_size;
	int num_embedded_objects;
	double session = session_request(mu_session, lambda_session);
	int i = 0;
	int j = 0;
	for(; i < session; i++) {
		html_page = html_page_size(mu_html, sigma_html, alfa_html);
		//setProcessClass???
		web_client(html_page);
		num_embedded_objects = object_per_request(alfa_obj);
		for(j=0; j < num_embedded_objects; j++) {
			embedded_object_size = embedded_object_size(mu_emb, sigma_emb);
			//setProcessClass????
		}
	}
	csim_terminate();
}
