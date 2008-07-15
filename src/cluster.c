#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include<string.h>
#include<csim.h>
#include "gaussiana_inversa.h"
#include "client.h"

extern double mu_session;
extern double lambda_session;
extern double alfa_tt;
extern double alfa_obj;
extern double mu_html;
extern double sigma_html;
extern double alfa_html;
extern double mu_emb;
extern double sigma_emb;

extern STREAM sess_req_1;
extern STREAM sess_req_2;
extern STREAM user_tt;
extern STREAM object_req;
extern STREAM html_1;
extern STREAM html_2;
extern STREAM obj_size;

void cluster(double *array, int array_length, int k)
{
	int i= 0;
	int j = 0;
	int iter = 0;
	int counter = 0;
	int converged = 0;
	double distance = 0.0;
	double distance_temp = 0.0;
	double min = array[0];
	double max = array[0];
	double *centroids = (double*)malloc(sizeof(double)*k) ;
	double *old_centroids = (double*)malloc(sizeof(double)*k);
	int *num_data_per_cluster = (int*)malloc(sizeof(long)*k);
	double *total_data_per_cluster = (double*)malloc(sizeof(double)*k);
	//calcolo del minimo e del massimo
	for(i=1;i < array_length; i++) {
		if(array[i] > max)
			max = array[i];
		if(array[i] < min)
			min = array[i];
	}
	//assegnazione dei primi due centroidi
	centroids[0] = min;
	printf("centroide min %lf\n", centroids[0]);
	centroids[k-1] = max;
	printf("centroide max %lf\n", centroids[2]);
	//calcolo degli altri centroidi in caso k>2 (forse da rivedere la formula)
	if(k>2) {
		for(i=1; i < k-1; i++) {
			centroids[i] = (max-min)/(k-1) * i;
		}
	}
	printf("centroide medio %lf\n", centroids[1]);
	while(!iter) {

		//assegnazione dei dati ad ogni cluster
		for(i=0; i < array_length; i++) {
			distance = fabs(centroids[0] - array[i]);

			for(j=1; j < k; j++) {

				distance_temp = fabs(centroids[j]-array[i]);
				if(distance > distance_temp) {
					distance = distance_temp;
					counter = j;
				}
			}

			num_data_per_cluster[counter] += 1;
			total_data_per_cluster[counter] += array[i];
			distance = 0.0;
			j = 0;
			counter = 0;
		}
		//step per tenere memoria dell'iterazione precedente
		for(i=0; i < k; i++) {
			old_centroids[i] = centroids[i];
		}
		printf("\n");
		//aggiornamento dei centroidi in seguito al primo step e confronto con la precedente iterazione
		for(i=0; i < k; i++) {

			centroids[i] = total_data_per_cluster[i] / ((double)num_data_per_cluster[i]); 
			printf("centroide: %lf - tot_dati: %d\t", centroids[i], num_data_per_cluster[i]);
			if( fabs(centroids[i]-old_centroids[i]) < pow(10,-10) ) {  //valore epsilon = 10^-15
				converged++;
			}
		}
		if(converged == k) {
			iter = 1;
		}
		converged = 0;
		memset(num_data_per_cluster,0,k*sizeof(double));
		memset(total_data_per_cluster,0,k*sizeof(double));
	}
}



void sim(int argc, char **argv)
{
	sess_req_1 = create_stream();
	reseed(sess_req_1, SEED);
	sess_req_2 = create_stream();
	reseed(sess_req_2, SEED);
	user_tt = create_stream();
	reseed(user_tt, SEED);
	object_req = create_stream();
	reseed(object_req, SEED);
	html_1 = create_stream();
	reseed(html_1, SEED);
	html_2 = create_stream();
	reseed(html_2, SEED);
	obj_size = create_stream();
	reseed(obj_size, SEED);

	create("prova");
	int array_length= 10000000;
	double *array = (double*)malloc(sizeof(double)*array_length);
	double session;
	int objects;
	int i=0;
	int j=0;
	int k=0;
	while(i<array_length) {
		session = session_request(mu_session, lambda_session);
		for(j=0; j < session; j++) {
			if( i < array_length) {
				array[i] = html_page_size(mu_html, sigma_html, alfa_html);
			}
			i++;
			objects = object_per_request(alfa_obj);
			for(k=0; k<objects; k++){
				if(i<array_length)
					array[i] = embedded_object_size(mu_emb, sigma_emb);
				i++;
			}
		}
	}
	cluster(array,array_length,3);

	int m=0;
	FILE *fd_file;
	char *pathname = (char*)malloc(128);
	sprintf(pathname, "docs");
	fd_file = fopen(pathname, "w");
	for(m=0;m<array_length;m++) {
		fprintf(fd_file, "%g\n", array[m]);
	}
	fclose(fd_file); 


}

