#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<csim.h>
#include "gaussiana_inversa.h"
#include "client.h"
#include "common.h"

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
extern TABLE rtime;
extern METER lambda;
extern CLASS requestClasses[K];

extern STREAM sess_req_1;
extern STREAM sess_req_2;
extern STREAM user_tt;
extern STREAM object_req;
extern STREAM html_1;
extern STREAM html_2;
extern STREAM obj_size;
extern STREAM p_hit_proxy;

extern double client_response_time;
extern double **observations;
extern int observed_sample;
extern int maxObservation;

TABLE resptime;

int	WELCH_N = 0,
	 	WELCH_M = 0,
	 	WELCH_W = 0;

char * output_file_name;

//Parsa i parametri passati da linea di comando
void parse_command_line(int argc, char *argv[]){

	if(argc != 5){
		printf("ERRORE!!!\nUtilizzo: ");
		printf("./transient <WELCH_N> <WELCH_M> <WELCH_W> <file_output>\n");
		exit(1);
	}
	
	WELCH_N = atoi(argv[1]);
	WELCH_M = atoi(argv[2]);
	WELCH_W = atoi(argv[3]);	
	output_file_name = argv[4];
	
	if(WELCH_W > WELCH_M/2){
		printf("ERRORE!!!\nIl valore della finestra W deve essere minore della parte intera di M/2.\n\n");
		exit(1);
	}
	
	
	return;	
	
}

//Simulazione del transiente
void sim(int argc, char *argv[]){

int clientID=0,
	 cont=0,
	 i,s,
	 n_repl=0,
	 m_repl=0;
	 
	 parse_command_line(argc, argv);
	 
double intT,
		 *averaged_process,
		 sum=0.0,
		 *moving_average;

FILE * mov_avg_fd;
	maxObservation = WELCH_M;
	observed_sample = 1;
	create("sim");
	
	max_processes(MAX_PROCESSES);	//numero massimo dei processi in giro nella rete	
	max_facilities(MAX_FACILITIES);
	max_servers(MAX_SERVERS);
  inLink = facility("inLink");
	outLink = facility("outLink");
	LS1 = facility("LS1");
	LS2 = facility("LS2");
	CPU_web_switch = facility("CPU_web_switch");
	L2 = facility("L2");
	facility_set(cpuWS, "cpuWS", NUM_SERVER);
	facility_set(diskWS, "diskWS", NUM_SERVER*NUM_DISK);
	facility_set(LW2, "LW2", NUM_SERVER);
	resptime = table("System Response Time"); // table intialization
	
	char className[20];
	WebSwitch = box("Web Switch");
	WebServer = box("Web Server");
	
	lambda = meter("Arrival Rate");
	max_classes(MAX_CLASSES);	//viene assegnato il numero massimo di classi per evitare l'errore "TOO MANY PROCESSES CLASS"
	for(cont=0; cont<NUM_CLASSES; cont++){
    className[0] = '\0';
		sprintf(className, "Classe%d", cont);
		requestClasses[cont] = process_class(className);

	}
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
		p_hit_proxy = create_stream();
		reseed(p_hit_proxy, (int)simtime()*8+i);

	collect_class_facility_all();
	
	rtime=table("Response Time");
	
	averaged_process = malloc((WELCH_M +1)*sizeof(double));
	moving_average=malloc((WELCH_M - WELCH_W +1)*sizeof(double));
	observations = (double**)malloc(WELCH_N * sizeof(double*));
	for(i=0; i<WELCH_N; i++){
		observations[i] = (double*)malloc(WELCH_M * sizeof(double));
	}

	
	//PASSO 1: vengono effettuate WELCH_N repliche di lunghezza WELCH_M ognuna. I risultati
	//			  sono inseriti nella matrice sample_matrix.
	//	  	  Il tempo di risposta percepito dal client viene campionato ogni SAMPLE_TIME secondi
	//	     Il generatore di numeri casuali non viene mai azzerato
	
	//ciclo di generazione delle richieste


	printf("n_repl =%d, m_repl =%d\n", n_repl, m_repl);
	while(n_repl < WELCH_N){
	observed_sample = 1;
		while(observed_sample < WELCH_M){

			intT=exponential(1/(double)(ARRIVAL));
			hold(intT);		//think time
			web_session(clientID, RANDOM, 1, n_repl);	
			clientID++;		
		}
		printf("Replication n° %d terminated\n",n_repl);
		//ogni volta che viene terminata una replica è necessario
		//riavviare le risorse
		wait(event_list_empty);
		reset();		
		reseed(sess_req_1, (int)simtime()+i);
		reseed(sess_req_2, (int)simtime()*2+i);
		reseed(user_tt, (int)simtime()*3+i);
		reseed(object_req, (int)simtime()*4+i);
		
		reseed(html_1, (int)simtime()*5+i);
		reseed(html_2, (int)simtime()*6+i);
		reseed(obj_size, (int)simtime()*7+i);
		reseed(p_hit_proxy, (int)simtime()*8+i);
		n_repl++;	
	}	
	
	//PASSO 2: Viene riempito l'array averaged_process con la media ottenuta dividendo
	//per WELCH_N, ovvero il numero di repliche effettuate, la somma di tutte le repliche 
	//per un dato indice appartenente all'insieme WELCH_M
	
	for(m_repl = 0; m_repl < WELCH_M; m_repl++){
	
		for(n_repl=0;n_repl<WELCH_N; n_repl++) {
		  sum += observations[n_repl][m_repl];
		}
			
		
		averaged_process[m_repl+1] = sum/(double)WELCH_N;
		
		sum = 0.0;
	}
	
		//PASSO 3: Calcolo dell'array Moving Average	
	
	for(i = 1; i <= (WELCH_M - WELCH_W); i++){
		sum = 0.0;
		if(i <= WELCH_W){
			for(s = -(i-1); s <= (i-1); s++)
				sum = sum + averaged_process[i+s];
			
			moving_average[i] = sum/(double)(2*i - 1);
		
		}
		else{
			for(s = -WELCH_W; s <= WELCH_W; s++)
				sum = sum + averaged_process[i+s];
			
			moving_average[i] = sum/(double)(2*WELCH_W + 1);
		}
	
	}	
	

	//PASSO 4: scrittura dell'array moving average su file che sarà utile poi per graficare
	//	   i valori e determinare il valore l (lunghezza del transiente).
	
	
	mov_avg_fd = fopen(output_file_name, "w");
	
	for(i = 1; i <= (WELCH_M - WELCH_W); i++)
		
		fprintf(mov_avg_fd, "%g\n", moving_average[i]);
	
	
	fclose(mov_avg_fd);	
	
	printf("Il file %s è stato riempito con l'array Moving Average.\n\n", output_file_name);

	free(averaged_process);
	free(moving_average);
	csim_terminate();

}



