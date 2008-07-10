#ifndef _GAUSSIANA_INVERSA_H
#define _GAUSSIANA_INVERSA_H

#include<math.h>
#include <csim.h>

double calc_mean_lognormal(double mu, double sigma) ;

double calc_stddev_lognormal(double mu, double sigma);

int session_request(double mu, double lambda);   //dimensionare come un intero???

double user_think_time(double alfa);

int object_per_request(double alfa);

double html_page_size(double mu, double sigma, double alfa);

double embedded_object_size(double mu, double sigma);

#endif
