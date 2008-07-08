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

/*
double session_request(double mu, double lambda)   //dimensionare come un intero???
{
 double x; 
 double v = normal(0,1);
 double y = pow(v,2);
 double x1 = 0;
 x1 = mu + ((pow(mu,2)*y)/(2*lambda)) - ((mu/(2*lambda)) * sqrt(4*mu*lambda*y+pow(mu,2)*pow(y,2)));
 double u = uniform(0,1);
 double temp = mu/(mu+x1);
 if(u <= temp) {
	x = x1;
 } 
 else
	x = pow(mu,2)/x1;

 return x;
}

double user_think_time(double alfa)
{
  double x = 0.0;
  while(x < 1) {
    x = pareto(alfa);
  }
  return x;
}

int object_per_request(double alfa)
{
  int x = 0;
  while(x < 2) {
    x = (int)(round(pareto(alfa)));
  }
  return x;
}

double html_page_size(double mu, double sigma, double alfa)
{
  double x = 0.0;
  int k = 10240; //dimensionarlo in KB???
  x = lognormal(mu,sigma);
  if(x<k)
    return x;
  else {
    x = pareto(alfa);
    while(x < k) {
       x = pareto(alfa);
    }	 
  }
  return x;
}

double embedded_object_size(double mu, double sigma) 
{
  return lognormal(mu,sigma);
}
*/

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
			if( fabs(centroids[i]-old_centroids[i]) < pow(10,-10) ) {  //valore epsilon = 10^-15 (col main di prova non va, se si abbassa epsilon funziona
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
	create_stream();
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

