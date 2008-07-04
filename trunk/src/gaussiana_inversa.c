#include "gaussiana_inversa.h"

//double mu = 3.86;
//double lambda = 9.46;

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
    x = (int)round(pareto(alfa));
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
 double x = 0.0;
 while(x <= 0.0) {
   x = lognormal(mu,sigma);
 }
 return x;
}
