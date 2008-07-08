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
	for(; i < iterations; i++) {
		osservazioni[i] = (double*)malloc(sizeof(double)*num_observations);
	}
	//qui manca il cuore del transient e il calcolo della prima media (magari la media come metodo a parte)
	
}
