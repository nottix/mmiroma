#include "gaussiana_inversa.h"

STREAM sess_req_1;
STREAM sess_req_2;
STREAM user_tt;
STREAM object_req;
STREAM html_1;
STREAM html_2;
STREAM obj_size;
STREAM p_hit_proxy;

double calc_mean_lognormal(double mu, double sigma) 
{
	double sum = mu+(pow(sigma,2)/2);
	return exp(sum);
}

double calc_stddev_lognormal(double mu, double sigma)
{
	double sum = (exp(pow(sigma,2))-1)*exp(2*mu+pow(sigma,2));
	return sqrt(sum);
}

int session_request(double mu, double lambda)   //dimensionare come un intero???
{
	int x; 
	double v = stream_normal(sess_req_1,0,1);
	double y = pow(v,2);
	double x1 = 0;
	x1 = mu + ((pow(mu,2)*y)/(2*lambda)) - ((mu/(2*lambda)) * sqrt(4*mu*lambda*y+pow(mu,2)*pow(y,2)));
	double u = stream_uniform(sess_req_2,0,1);
	double temp = mu/(mu+x1);
	if(u <= temp) {
		x = (int)round(x1);
	}
	else
		x = (int)round(pow(mu,2)/x1);

	return x;
}

double user_think_time(double alfa)
{
	double x = 0.0;
	while(x < 1) {
		x = stream_pareto(user_tt, alfa);
	}
	return x;
}

int object_per_request(double alfa)
{
	int x = 0;
	while(x < 2) {
		x = (int)round(stream_pareto(object_req, alfa));
	}
	return x;
}

double html_page_size(double mu, double sigma, double alfa)
{
	double x = 0.0;
	int k = 10240; //dimensionarlo in KB???
	x = stream_lognormal(html_1, calc_mean_lognormal(mu,sigma),calc_stddev_lognormal(mu,sigma));
	if(x<k)
		return x;
	else {
		x = stream_pareto(html_2, alfa);
		while(x < k) {
			x = stream_pareto(html_2, alfa);
		}	 
	}
	return x;
}

double embedded_object_size(double mu, double sigma) 
{
	double x = 0.0;
	while(x <= 0.0) {
		x = stream_lognormal(obj_size, calc_mean_lognormal(mu,sigma),calc_stddev_lognormal(mu,sigma));
	}
	return x;
}
