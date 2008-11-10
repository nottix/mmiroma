#ifndef _WORKLOAD_H
#define _WORKLOAD_H

#include <math.h>
#include <csim.h>

#ifndef _ISOC99_SOURCE
#define _ISOC99_SOURCE
#endif

//calcolo della media per una distribuzione lognormale
double calc_mean_lognormal(double mu, double sigma) ;

//calcolo della deviazione standard per una distribuzione lognormale
double calc_stddev_lognormal(double mu, double sigma);

//algoritmo di Michael/Schucany/Haas per la generazione di valori tramite la gaussiana inversa
int session_request(double mu, double lambda);   

//generazione di valori per il think time dell'utente (Pareto)
double user_think_time(double alfa);

//generazione del numero di oggetti per richiesta (Pareto)
int object_per_request(double alfa);

//generazione della dimensione della pagina html (Pareto e Lognormale)
double html_page_size(double mu, double sigma, double alfa);

//generazione della dimensione degli embedded object (Lognormale)
double embedded_object_size(double mu, double sigma);

#endif
