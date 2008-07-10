#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<csim.h>
#include "gaussiana_inversa.h"

double mu_session = 3.86;
double lambda_session = 9.46;
double alfa_tt = 1.4;
double alfa_obj = 1.33;
double mu_html = 7.63;
double sigma_html = 1.001;
double alfa_html = 1;
double mu_emb = 8.215;
double sigma_emb = 1.46;

extern STREAM sess_req_1;
extern STREAM sess_req_2;
extern STREAM user_tt;
extern STREAM object_req;
extern STREAM html_1;
extern STREAM html_2;
extern STREAM obj_size

void cluster(double *array, int array_length, int k)
{
  int i= 0;
  int j = 0;
  int iter = 0;
  int counter = 0;
  int converged = 0;
  double distance;
  double min = array[0];
  printf("array[0]: %lf\n", array[0]);
  double max = array[0];
  double centroids[k];
  double old_centroids[k];
  double num_data_per_cluster[k];
  double total_data_per_cluster[k];
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
 
  while(!iter) {
    //assegnazione dei dati ad ogni cluster
  	for(i=0; i < array_length; i++) {
			distance = fabs(centroids[0] - array[i]);
    	for(j=1; j < k; j++) {
			  double distance_temp = fabs(centroids[j]-array[i]);
				if(distance > distance_temp) {
					distance = distance_temp;
					counter = j;
				}
			}
			num_data_per_cluster[counter] += 1;
			total_data_per_cluster[counter] += array[i];
			distance = 0;
			j = 0;
			counter = 0;
		}
		//step per tenere memoria l'iterazione precedente
		for(i=0; i < k; i++) {
			old_centroids[i] = centroids[i];
		}
		printf("\n");
		//aggiornamento dei centroidi in seguito al primo step e confronto con la precedente iterazione
		for(i=0; i < k; i++) {
		  if(num_data_per_cluster[i] != 0) { //prova, in realtà non dovrebbe mai accadere che un centroide resti vuoto
				centroids[i] = total_data_per_cluster[i] / num_data_per_cluster[i]; 
		  }
		  else
		  	centroids[i] = 0;
		  printf("centroide: %lf - tot_dati: %lf\t", centroids[i], num_data_per_cluster[i]);
			if( fabs(centroids[i]-old_centroids[i]) < pow(10,-10) ) {  //valore epsilon = 10^-15
				converged++;
			}
		}
		if(converged == k) {
			iter = 1;
		}
		memset(num_data_per_cluster,0,k*sizeof(double));
		memset(total_data_per_cluster,0,k*sizeof(double));
	}
}

void sim(int argc, char **argv)
{
	sess_req_1 = create_stream();
	reseed(sess_req1, SEED);
	sess_req_2 = create_stream();
	reseed(sess_req2, SEED);
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
	int array_length= 1000000;
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
}

