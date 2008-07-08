#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<csim.h>
#include "gaussiana_inversa.h"
#include "client.h"

/*
double* calc_mobile_mean(int W, int iterations, int num_observations, double *observation_average)
{
  double *media_mobile = (double*)malloc(sizeof(double)*(num_observations-W+1));
	int i=0;
	int temp = 0;
	for(i=0; i <= (num_observations-W); i++) {
		if(i <= W) {
			temp = (-1)*(i-1);
			for(; temp < i-1; temp++) {
				media_mobile[i] += observation_average[temp];
			}
			media_mobile[i] /= (double)(2*i-1);
		}
		else {
			temp = (-1) * W;
			for(; temp < W; temp ++) {
				media_mobile[i] += observation_average[temp];
			}
			media_mobile[i] /= (double)(2*W+1);
		}
	}
	return media_mobile;
}

void calc_transient(int num_observations, int iterations, int W)
{
	double *media_osservazioni = (double*)malloc(sizeof(double)*(num_observations+1));
	double *media_mobile = (double*)malloc(sizeof(double)*(num_observations-W+1));
	double **osservazioni;
	int i=0;
	int j=0;
	int client_id = 0;
	int num_campioni;
	int variante = 1;
	for(; i < iterations; i++) {
		osservazioni[i] = (double*)malloc(sizeof(double)*num_observations);
	}
	//qui manca il cuore del transient e il calcolo della prima media (magari la media come metodo a parte)
	for(i=0; i<iterations; i++){ // for each iteration
		num_campioni = 1;
		while(num_campioni<=num_observations){
			hold(exponential(1/(double)150));
			webSession(client_id/*, i, TRUE,*/ variante); // create a new session
			client_id++;
		}
		wait(event_list_empty); // wait for the end of all events
		reset(); // reset the collected statistics
		//reseedStream(i); boh???
		for(j=1; j < num_observations; i++) { //perchè uno?? l'array parte da zero
			media_osservazioni[j] = osservazioni[i][j];
		}
	}	
	for(i=1; i < num_observations; i++) {
		media_osservazioni[i] /= (double)iterations;
	}
	media_mobile = calc_mobile_mean(W, iterations, num_observations, media_osservazioni);
}
*/
/*****************************************************************************************/
//ALTERNATIVA


int	WELCH_N = 0,
	 	WELCH_M = 0,
	 	WELCH_W = 0;

char * output_file_name;

void sim(int argc, char *argv[]){

int clientID=0,
	 resetted=0,
	 cont=0,
	 i,s,
	 n_repl=0,
	 m_repl=0;
	 
	 parse_command_line(argc, argv);
	 
double intT,
		 **sample_matrix,
		 *averaged_process,
		 sum=0.0,
		 *moving_average,
		 old_simtime=0.0,
		 current_simtime=0.0;

long start_simulation, end_simulation;

char strbuf[30];

FILE * mov_avg_fd;

	create("sim");
	
	max_processes(100000000);	//numero massimo dei processi in giro nella rete	
	
	/*facility_set(cpuWS, "cpuWS", N);
	
	cpuFS = facility("cpuFS");
	
	facility_set(diskFS, "diskFS", P);
	
	LAN=facility("LAN");

	inLink=facility("inLink");

	outLink=facility("outLink");
  */
  inLink = facility("inLink");
	outLink = facility("outLink");
	CPU_web_switch = facility("CPU_web_switch");
	L2 = facility("L2");
	facility_set(cpuWS, "cpuWS", NUM_SERVER);
	facility_set(diskWS, "diskWS", NUM_SERVER*NUM_DISK);
	//facility_set(LW2_out, "LW2_out", NUM_SERVER);
	
	resptime = table("System Response Time"); // table intialization
	
	char className[20];
	className[0] = '\0';
	
	//	webSwitch = box("Web Switch");
	WebServer = box("Web Server");
	
	lambda = meter("Arrival Rate");
	max_classes(10);	//viene assegnato il numero massimo di classi per evitare l'errore "TOO MANY PROCESSES CLASS"
	for(cont=0; cont<NUM_CLASS; cont++){

		requestClasses[cont] = process_class(Classe[cont]);

	}

	collect_class_facility_all();
	
	rtime=table("Response Time");
	sample_matrix=malloc(WELCH_N *sizeof(double*));
	for(i=0;i<WELCH_N;i++)
		*(sample_matrix+i) = malloc((WELCH_M * sizeof(double)));
	averaged_process = malloc((WELCH_M +1)*sizeof(double));
	moving_average=malloc((WELCH_M - WELCH_W +1)*sizeof(double));
	
	//PASSO 1: vengono effettuate WELCH_N repliche di lunghezza WELCH_M ognuna. I risultati
	//			  sono inseriti nella matrice sample_matrix.
	//	  	  Il tempo di risposta percepito dal client viene campionato ogni SAMPLE_TIME secondi
	//	     Il generatore di numeri casuali non viene mai azzerato
	
	//ciclo di generazione delle richieste

	printf("n_repl =%d, m_repl =%d\n", n_repl, m_repl);
	while(n_repl < WELCH_N){	
	old_simtime = simtime();
	m_repl=0;
		while(m_repl < WELCH_M){

			intT=stream_exponential(request_stream, 1/(double)(ARRIVAL));
			hold(intT);		//think time
			webclient(clientID, resetted);	//invio richiesta //da modificare
			clientID++;		
			current_simtime = simtime();
			if((current_simtime-old_simtime)>SAMPLE_TIME){
			//printf("campionamento %d effettuato\n", m_repl);
					sample_matrix[n_repl][m_repl]=CLIENT_RESPONSE_TIME;
					m_repl++;
					old_simtime = simtime();
			}
		}
		printf("Replication n° %d terminated\n",n_repl);
		//ogni volta che viene terminata una replica è necessario
		//riavviare le risorse
		reset();		
		
		n_repl++;	
	}	
	
	//PASSO 2: Viene riempito l'array averaged_process con la media ottenuta dividendo
	//per WELCH_N, ovvero il numero di repliche effettuate, la somma di tutte le repliche 
	//per un dato indice appartenente all'insieme WELCH_M
	
	for(m_repl = 0; m_repl < WELCH_M; m_repl++){
	
		for(n_repl=0;n_repl<WELCH_N; n_repl++)
			sum += sample_matrix[n_repl][m_repl];
		
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
	
	
	close(mov_avg_fd);	
	
	printf("Il file %s è stato riempito con l'array Moving Average.\n\n", output_file_name);

	for(i=0;i<WELCH_N;i++)
		free(*(sample_matrix+i));
	free(sample_matrix);
	free(averaged_process);
	free(moving_average);
	csim_terminate();

}

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
