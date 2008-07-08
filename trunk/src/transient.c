#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<csim.h>
#include "gaussiana_inversa.h"
#include "client.h"

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
